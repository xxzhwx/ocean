#include <stdio.h>

extern "C" {
#include "curl/curl.h"
}

#include "lfwk.h"
using namespace lfwk;

int main(int argc, char *argv[])
{
    printf("GameWorld\n");

    Service* service = new Service("test-service");
    service->Startup();

    LFWK_Sleep(3000);

    service->Shutdown();
    delete service;

    CURL* client = curl_easy_init();
    curl_easy_setopt(client, CURLOPT_URL, "http://www.baidu.com");
    curl_easy_perform(client);
    curl_easy_cleanup(client);

    return 0;
}
