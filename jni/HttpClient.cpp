/**
 * http client
 * 
 * tomken
**/

#include "HttpClient.h"
#include "HttpPostChunk.h"
#include "HttpPostInputChunk.h"
#include "HttpPostFileChunk.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <netdb.h>

#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

#define SA      struct sockaddr
#define MAXLINE 4096

HttpClient::HttpClient() {
	mEndBoundary = "--";
	mEndBoundary += HTTP_BOUNDARY;
	mEndBoundary += "--\r\n";
}

HttpClient::~HttpClient() {
}

int HttpClient::get(string url) {
	if (!parseUrl(url))
		return -1;
	
	if (!connectServer()) {
		return -2;
	}
	
	stringstream header;
	header << "GET " << mPath << " HTTP/1.1\r\n";
	header << "Host: " << mHost << "\r\n";
	header << "Accept: text/plain, */*\r\n";
	header << "User-Agent: Mozilla/5.0 (Windows; U; Windows NT 5.1; en-US; rv:1.7.6)\r\n";
	header << "Connection: close\r\n";
	header << "\r\n";
	
	write(mSocket, header.str().c_str(), header.str().length());
	
	readReceiver();
	close(mSocket);
	
	return 0;
}

int HttpClient::post(string url, vector<HttpPostChunk*>& chunks) {
	if (!parseUrl(url))
		return -1;
	
	if (!connectServer()) {
		return -2;
	}
	
	int contentLen = 0;
	
	vector<HttpPostChunk*>::iterator it;
	for ( it=chunks.begin() ; it < chunks.end(); it++ ) {
		contentLen += (*it)->size();
	}
	
	contentLen += mEndBoundary.length();
	
	stringstream header;
	header << "POST " << mPath << " HTTP/1.1\r\n";
	header << "Host: " << mHost << "\r\n";
	header << "Content-Type: multipart/form-data; boundary=" << HTTP_BOUNDARY << "\r\n";
	header << "Content-Length: " << contentLen << "\r\n";
	header << "Accept: text/plain, */*\r\n";
	header << "User-Agent: Mozilla/5.0 (Windows; U; Windows NT 5.1; en-US; rv:1.7.6)\r\n";
	header << "Connection: close\r\n";
	header << "\r\n";
	
//	cout << "-- begin post -------------" << endl;
//	cout << header.str();
	write(mSocket, header.str().c_str(), header.str().length());
	
	for ( it=chunks.begin() ; it < chunks.end(); it++ ) {
		(*it)->writeSocket(mSocket);
	}
	
	write(mSocket, mEndBoundary.c_str(), mEndBoundary.length());
//	cout << mEndBoundary;
//	cout << "-- end post -------------" << endl;
	
	readReceiver();
	close(mSocket);
	
	return 0;
}

int HttpClient::head(string url) {
	if (!parseUrl(url))
		return -1;
	
	if (!connectServer()) {
		return -2;
	}
	
	stringstream header;
	header << "HEAD " << mPath << " HTTP/1.1\r\n";
	header << "Host: " << mHost << "\r\n";
	header << "Accept: text/plain, */*\r\n";
	header << "User-Agent: Mozilla/5.0 (Windows; U; Windows NT 5.1; en-US; rv:1.7.6)\r\n";
	header << "Connection: close\r\n";
	header << "\r\n";
	
	write(mSocket, header.str().c_str(), header.str().length());
	readReceiver();
	close(mSocket);
	
	return 0;
}

void HttpClient::readReceiver() {
	ssize_t n;
	char recvline[MAXLINE+1];
	while ((n = read(mSocket, recvline, MAXLINE)) > 0) {
		recvline[n] = '\0';
		cout << recvline;
	}
}

bool HttpClient::parseUrl(string url) {
	string proto = url.substr(0, 7);
	if (proto.compare("http://") != 0)
		return false;
	
	size_t idx;
	url = url.substr(7);
	
	string domain;
	idx = url.find('/');
	if (idx == string::npos) {
		domain = url;
		mPath = "/";
	} else {
		domain = url.substr(0, idx);
		mPath = url.substr(idx);
	}
	
	idx = url.find(':');
	if (idx == string::npos) {
		mHost = domain;
		mPort = 80;
	} else {
		mHost = domain.substr(0, idx);
		mPort = atoi(domain.substr(idx+1).c_str());
	}
	cout << "host:" << mHost << endl;
	cout << "port:" << mPort << endl;
	cout << "path:" << mPath << endl;
	
	return true;
}

bool HttpClient::connectServer() {
	struct sockaddr_in servaddr;
	struct hostent *hptr;
	
	if ((hptr = gethostbyname(mHost.c_str())) == NULL) {
		fprintf(stderr, " gethostbyname error for host: %s: %s",
			mHost.c_str(), hstrerror(h_errno));
		return false;
	}
	
	char **pptr;
	char str[50];
	if (hptr->h_addrtype == AF_INET
	    && (pptr = hptr->h_addr_list) != NULL) {
		printf("address: %s\n",
		       inet_ntop(hptr->h_addrtype, *pptr, str,
				 sizeof(str)));
	} else {
		fprintf(stderr, "Error call inet_ntop \n");
	}
	
	mSocket = socket(AF_INET, SOCK_STREAM, 0);
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(mPort);
	inet_pton(AF_INET, str, &servaddr.sin_addr);
	
	if (connect(mSocket, (SA *) & servaddr, sizeof(servaddr)) < 0) {
		cout << "Connect " << mHost << ":" << mPort << " error!" << endl;
		return false;
	}
	
	return true;
}

int main(int argc, char* argv[]) {
	vector<HttpPostChunk*> fields;
	fields.push_back(new HttpPostInputChunk("server", "server_ip"));
	fields.push_back(new HttpPostInputChunk("leader", "xxxx"));
	fields.push_back(new HttpPostFileChunk("data", "/data/local/tmp/test.txt"));

	HttpClient* http = new HttpClient();
	http->post("http://server_ip/upload.php", fields);
//	http->get("http://www.baidu.com/index.html");
//	http->get("http://www.baidu.com:8080/index.html");
	
	return 0;
}

// end of file
