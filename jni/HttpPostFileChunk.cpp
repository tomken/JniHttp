/**
 * http client
 * 
 * tomken
**/

#include "HttpPostFileChunk.h"
#include "HttpClient.h"

#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>

#define MAXLINE 4096

/**
 * Content-Disposition: form-data; name="userfile"; filename="classes.dex"
 * Content-Type: application/octet-stream
**/
HttpPostFileChunk::HttpPostFileChunk(string name, string path) {
	mPath = path;
	mFd = -1;
	
	string basename = getBasename();
	
	mData << "--" << HTTP_BOUNDARY << "\r\n";
	mData << "Content-Disposition: form-data; name=\"" << name << "\"; filename=\"" << basename << "\"\r\n";
	mData << "\r\n";
//	mData << value;
//	mData << "\r\n";
}

HttpPostFileChunk::~HttpPostFileChunk() {
	if (mFd > 0) {
		close(mFd);
	}
}

int HttpPostFileChunk::size() {
	if (mFd < 0) {
		mFd = open(mPath.c_str(), O_RDONLY, 0);
		if (mFd < 0) {
			return -1;
		}
	}
	
	int size = getFileSize();
	if (size <= 0) {
		return -1;
	}
	
	size += mData.str().length();
	size += 2;
	
	return size;
}

int HttpPostFileChunk::writeSocket(int sock) {
	write(sock, mData.str().c_str(), mData.str().length());
	
	ssize_t n;
	char recvline[MAXLINE+1];
	while ((n = read(mFd, recvline, MAXLINE)) > 0) {
		recvline[n] = '\0';
		write(sock, recvline, n);
	}
	
	write(sock, "\r\n", 2);
	return 0;
}

int HttpPostFileChunk::getFileSize() {
	off_t start, end;
	start = lseek(mFd, 0L, SEEK_CUR);
    end = lseek(mFd, 0L, SEEK_END);
    (void) lseek(mFd, start, SEEK_SET);
	
	if (start == (off_t) -1 || end == (off_t) -1) {
        return -1;
    }
	
	return (end - start);
}

string HttpPostFileChunk::getBasename() {
	string name = mPath;
	size_t found = name.rfind('/');
	if (found != string::npos) {
		name = name.substr(found + 1);
	}
	
	found = name.rfind('\\');
	if (found != string::npos) {
		name = name.substr(found + 1);
	}
	
	return name;
}

// end of file

