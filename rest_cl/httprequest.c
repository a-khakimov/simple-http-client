#include "httprequest.h"
#include <curl/curl.h>
#include <string.h>
#include <stdlib.h>

#define UNUSED(a) (void*)a

extern int verbose;


typedef struct {
    const char* readptr;
    size_t sizeleft;
} WriteThis;


static size_t read_data(void* dest, size_t size, size_t nmemb, void *userp)
{
    WriteThis* wt = (WriteThis*)userp;
    const size_t buffer_size = size * nmemb;

    if (wt->sizeleft) {
        size_t copy_this_much = wt->sizeleft;
        if (copy_this_much > buffer_size) {
            copy_this_much = buffer_size;
        }
        memcpy(dest, wt->readptr, copy_this_much);

        wt->readptr += copy_this_much;
        wt->sizeleft -= copy_this_much;
        return copy_this_much;
    }
    return 0;
}

static size_t write_data(void* data, size_t size, size_t nmemb, void *userp)
{
    UNUSED(data);
    UNUSED(userp);
    return size * nmemb;
}

long sendPOST(const char* url, const char* data, const size_t size)
{
    long response_code = -1;

    WriteThis wt;
    wt.readptr = data;
    wt.sizeleft = size;

    CURLcode res = curl_global_init(CURL_GLOBAL_DEFAULT);
    if (res != CURLE_OK) {
        fprintf(stderr, "curl_global_init() failed: %s\n", curl_easy_strerror(res));
        return -1;
    }

    CURL* curl = curl_easy_init();
    if (curl != (CURL*)NULL) {
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_POST, 1L);
        curl_easy_setopt(curl, CURLOPT_READFUNCTION, read_data);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
        curl_easy_setopt(curl, CURLOPT_READDATA, &wt);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, (long)wt.sizeleft);
        curl_easy_setopt(curl, CURLOPT_VERBOSE, verbose);

        res = curl_easy_perform(curl);
        if (res == CURLE_OK) {
            curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);
        } else {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        }
        curl_easy_cleanup(curl);
    }
    curl_global_cleanup();
    return response_code;
}
