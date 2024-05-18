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
