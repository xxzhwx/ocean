#include <stdio.h>

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

    return 0;
}
