/**
 * http client
 * 
 * tomken
**/

#ifndef __HTTP_POST_INPUT_CHUNK_H__
#define __HTTP_POST_INPUT_CHUNK_H__

#include "HttpPostChunk.h"

#include <string>
#include <sstream>

using namespace std;

class HttpPostInputChunk : public HttpPostChunk {
public:
	HttpPostInputChunk(string name, string value);
	~HttpPostInputChunk();
	
public:
	int size();
	int writeSocket(int sock);
	
private:
	stringstream  mData;

};

#endif // __HTTP_POST_INPUT_CHUNK_H__

