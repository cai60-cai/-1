1. SSL_CTX_use_certificate_chain_file error! 加载服务器证书链文件错了

    验证证书和密钥匹配的md5一样捏，说明匹配

    openssl verify -CAfile /home/user/Desktop/Cyber_Security_Code_Lab/lab1/7/root-cert.pem /home/user/Desktop/Cyber_Security_Code_Lab/lab1/7/server.pem
    验证证书链，发现过期了

    C = CN, ST = TianJin, L = TianJin, O = NanKai, OU = NanKai, CN = NKU
    error 10 at 1 depth lookup: certificate has expired
    C = CN, ST = HeBei, L = LangFang, O = DaChang, OU = ChengGuan, CN = 192.168.1.49
    error 10 at 0 depth lookup: certificate has expired

    那就重新生成一个吧

    openssl genrsa -out server-key.pem 2048
    openssl req -new -key server-key.pem -out server.csr
    openssl x509 -req -days 365 -in server.csr -signkey server-key.pem -out server.pem

    就按照前面的报错内容填就好了


    不知道为啥这个自签名的还过了根证书的验证hhhh

    但是后面呢http客户端过不了，只能忽略，毕竟默认情况下 libcurl 会拒绝连接自签名的



/*
服务器启动并监听端口。
客户端连接并发送请求。
服务器接收请求并解析。
服务器查找并读取文件。
服务器发送HTTP头和文件内容。
客户端接收文件内容。
*/

2. 基本运行
    ./MyWebServer
    curl -k https://localhost:8000
    发现没有index.html，因此自己写一个/home/WebServer/index.html
    想请求其他文件就在 /home/WebServer/test.txt   加上就行
    curl -k https://localhost:8000/test.txt
    输出过了


3. 检测大端小端
    在Web服务器中添加一个处理端点/endianness返回系统的字节序信息就可以

        在 HttpProtocol.cpp 中实现 HandleEndianness 函数
        在 CHttpProtocol::Analyze 函数中，添加对 /endianness 请求的处理
        确保 PREQUEST 结构体中有一个 BIO* 字段，用于 SSL BIO 操作。可以在 ClientThread 函数中添加 BIO 的初始化
        REQUEST 结构体中添加一个 BIO* 成员，用于处理 SSL BIO 操作

    运行就是curl -k https://localhost:8000/endianness，他就会返回大端小端


4. 实现 HTTP Server 的 HEAD、 GET 和 POST 方法

    添加那三个函数，在 Analyze 函数中处理 HEAD、GET 和 POST 请求就行
    common.h 文件中定义 METHOD_POST 标识


    运行就是 
    curl -k https://localhost:8000
    curl -I -k https://localhost:8000
    curl -X POST -k https://localhost:8000 -d "name=test"

    这一步的bug是出现了Content-Length: 0，估计是从 pReq->pHttpProtocol 中读取 Content-Length的问题，在request加上 int content_length；在 Analyze 函数中解析 Content-Length 并存储到 pReq->content_length；更新 HandlePost 函数以使用 pReq->content_length

5. bm算法

    运行curl -X POST -k https://localhost:8000 -d "This is a sensitive test string."


6. 实现HTTP Client 代码

    #ifndef HTTP_CLIENT_H
    #define HTTP_CLIENT_H

    #include <string>
    #include <unordered_map>
    #include <vector>

    // 写回调函数，用于接收服务器响应
    size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* s);

    // 发送POST请求的函数
    void sendPostRequest(const std::string& url, const std::string& data, int id);

    // 多线程执行HTTP请求
    void performRequests(const std::string& url, const std::string& data, int numRequests);

    #endif // HTTP_CLIENT_H



7. makefile修改就好了

