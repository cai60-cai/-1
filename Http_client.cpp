#include <iostream>
#include <string>
#include <thread>
#include <vector>
#include <curl/curl.h>
#include <chrono>
#include "Http_client.h"

//进行 HTTP 请求时接收响应数据//
//将接收到的数据拼接到 std::string 对象中，并返回接收到的数据总大小
//相当于构建器
size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* s) {
    size_t newLength = size * nmemb;
    try {
        s->append((char*)contents, newLength);
    } catch (std::bad_alloc& e) {
        // Handle memory problem
        return 0;
    }
    return newLength;
}

//发送请求用的
void sendPostRequest(const std::string& url, const std::string& data, int id) {
    CURL* curl;
    CURLcode res;
    std::string response;
    auto start = std::chrono::high_resolution_clock::now();

    curl_global_init(CURL_GLOBAL_DEFAULT);//全局初始化
    curl = curl_easy_init();
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_POST, 1L);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data.c_str());


        // // 忽略证书验证
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);


        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

        //执行 HTTP 请求
        res = curl_easy_perform(curl);
        std::cout<<"==========测试一下下=========="<<std::endl;
        if(res != CURLE_OK)
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
        else
            std::cout << "Thread " << id << " Response: " << response << std::endl;

        curl_easy_cleanup(curl);
    }
    curl_global_cleanup();

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;    //计算时间
    std::cout << "Thread " << id << " elapsed time: " << elapsed.count() << " seconds" << std::endl;
}

//像容器中添加新线程
void performRequests(const std::string& url, const std::string& data, int numRequests) {
    std::vector<std::thread> threads;

    for (int i = 0; i < numRequests; ++i) {
        threads.emplace_back(sendPostRequest, url, data, i);
    }

    for (auto& thread : threads) {
        if (thread.joinable()) {
            thread.join();//等待所有线程执行完毕
        }
    }
}

int main() {
    std::string url = "https://localhost:8000";
    std::string data = "This is a sensitive test string.";
    int numRequests = 10;

    performRequests(url, data, numRequests);

    return 0;
}
