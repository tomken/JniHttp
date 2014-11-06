/**
 * http client
 * 
 * tomken
**/

#ifndef __HTTP_POST_CHUNK_H__
#define __HTTP_POST_CHUNK_H__

class HttpPostChunk {
public:
	virtual int size() = 0;
	virtual int writeSocket(int sock) = 0;
};

#endif // __HTTP_POST_CHUNK_H__

