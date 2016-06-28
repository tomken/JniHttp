# JniHttp

简单的 HTTP 封装，支持GET、POST请求，支持表单POST

示例
···
vector<HttpPostChunk*> fields;
fields.push_back(new HttpPostInputChunk("server", "jeeble"));
fields.push_back(new HttpPostInputChunk("leader", "xxxx"));
fields.push_back(new HttpPostFileChunk("data", "/data/local/tmp/test.txt"));

HttpClient* http = new HttpClient();
http->post("http://jeeble.cn/upload.php", fields);
···
