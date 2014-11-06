/**
 * http client
 * 
 * tomken
**/

#ifndef __HTTP_POST_FILE_CHUNK_H__
#define __HTTP_POST_FILE_CHUNK_H__

#include "HttpPostChunk.h"

#include <string>
#include <sstream>

using namespace std;

class HttpPostFileChunk : public HttpPostChunk {
public:
	HttpPostFileChunk(string name, string path);
	~HttpPostFileChunk();
	
public:
	int size();
	int writeSocket(int sock);
	
private:
	int getFileSize();
	string getBasename();
	
private:
	string  mPath;
	int     mFd;
	
	stringstream  mData;

};

#endif // __HTTP_POST_FILE_CHUNK_H__

