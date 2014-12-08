#ifndef VS_HTTPFORMREQUEST_H
#define VS_HTTPFORMREQUEST_H

#include <common/VSCommon.h>
#include <vector>

struct curl_httppost;
struct curl_slist;

namespace vseg {

class HttpFormRequest {
public:
    HttpFormRequest();
    ~HttpFormRequest();
    
    HttpFormRequest(const HttpFormRequest&) = delete;
    HttpFormRequest& operator=(const HttpFormRequest&) = delete;

    void addString(const std::string &key, const std::string &value);
    void addBinary(const std::string &key, const std::vector<char> &data);
    void addBinary(const std::string &key, const char *data, int size);
    
    int perform(const std::string &url);
    
private:
    static void initCurl();
    
    curl_httppost *_formPost;
    curl_httppost *_lastPost;
};

}

#endif // VS_HTTPFORMREQUEST_H
