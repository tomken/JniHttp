/**
 * http client
 * 
 * tomken
**/

#include "HttpPostInputChunk.h"
#include "HttpClient.h"

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

HttpPostInputChunk::HttpPostInputChunk(string name, string value) {
	mData << "--" << HTTP_BOUNDARY << "\r\n";
	mData << "Content-Disposition: form-data; name=\"" << name << "\"\r\n";
	mData << "\r\n";
	mData << value;
	mData << "\r\n";
}

HttpPostInputChunk::~HttpPostInputChunk() {
}

int HttpPostInputChunk::size() {
	return mData.str().length();
}

int HttpPostInputChunk::writeSocket(int sock) {
//	cout << mData.str();
	return write(sock, mData.str().c_str(), mData.str().length());
}

// end of file
