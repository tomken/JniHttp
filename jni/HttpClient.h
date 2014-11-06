/**
 * http client
 * 
 * tomken
**/

#ifndef __HTTP_CLIENT_H__
#define __HTTP_CLIENT_H__

#include <string>
#include <iostream>
#include <sstream>
#include <map>
#include <list>
#include <vector>

using namespace std;

#define HTTP_BOUNDARY "---------7d4a6d158c9"

class HttpPostChunk;
class HttpClient {
public:
	HttpClient();
	~HttpClient();
	
	enum {
		EHTTPGet = 0,
		EHTTPPost,
		EHTTPHead
	};
	
public:
	int get(string url);
	int post(string url, vector<HttpPostChunk*>& chunks);
	int head(string url);

private:
	bool parseUrl(string url);
	bool connectServer();
	void readReceiver();
	
private:
	string mHost;
	int    mPort;
	string mPath;
	
	string mEndBoundary;
	
	int    mSocket;
};

#endif // __HTTP_CLIENT_H__

