#include "HttpFormRequest.h"
#include <common/ScopePtr.h>
#include <curl/curl.h>
#include <string>
#include <iostream>

namespace vseg {

void HttpFormRequest::initCurl() {
    static bool curlInited = false;
    
    if (!curlInited) {
        curl_global_init(CURL_GLOBAL_ALL);
        curlInited = true;
    }
}

HttpFormRequest::HttpFormRequest()
: _formPost(0), _lastPost(0)
{
    initCurl();
}

HttpFormRequest::~HttpFormRequest() {
    if (_formPost) {
        curl_formfree(_formPost);
    }
}

void HttpFormRequest::addString(const std::string &key, const std::string &value) {
    curl_formadd(
        &_formPost,
        &_lastPost,
        CURLFORM_COPYNAME, key.c_str(),
        CURLFORM_COPYCONTENTS, value.c_str(),
        CURLFORM_END);
}

void HttpFormRequest::addBinary(const std::string &key, const char *data, int size) {
    curl_formadd(
        &_formPost,
        &_lastPost,
        CURLFORM_COPYNAME, key.c_str(),
        CURLFORM_BUFFER, key.c_str(),
        CURLFORM_BUFFERPTR, data,
        CURLFORM_BUFFERLENGTH, size,
        CURLFORM_END);
}

void HttpFormRequest::addBinary(const std::string &key, const std::vector<char> &data) {
    std::cout << "ttpFormRequest::addBinary(): " << data.data() << "\t" << data.size() << std::endl;

    curl_formadd(
        &_formPost,
        &_lastPost,
        CURLFORM_COPYNAME, key.c_str(),
        CURLFORM_BUFFER, key.c_str(),
        CURLFORM_BUFFERPTR, data.data(),
        CURLFORM_BUFFERLENGTH, data.size(),
        CURLFORM_END);
}

int HttpFormRequest::perform(const std::string &url) {
    ScopePtr<CURL> curl(curl_easy_init(), [] (CURL *p) { curl_easy_cleanup(p); } );
 
    curl_easy_setopt(curl.get(), CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl.get(), CURLOPT_HTTPPOST, _formPost);
    
    CURLcode res = curl_easy_perform(curl.get());
    long httpCode;
    curl_easy_getinfo(curl.get(), CURLINFO_HTTP_CODE, &httpCode);
    
    return httpCode;
}

}
