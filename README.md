# JniHttp

简单的 HTTP 封装，支持GET、POST请求，支持表单POST

示例:

```cpp
vector<HttpPostChunk*> fields;
fields.push_back(new HttpPostInputChunk("server", "jeeble"));
fields.push_back(new HttpPostInputChunk("leader", "xxxx"));
fields.push_back(new HttpPostFileChunk("data", "test.txt"));

HttpClient* http = new HttpClient();
http->post("http://test.com/upload.php", fields);
```
