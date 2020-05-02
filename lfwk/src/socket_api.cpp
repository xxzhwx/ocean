#include "socket_api.h"

NS_BEGIN_LFWK

int LFWK_StartupSocketLib()
{
#ifdef _MSC_VER
    WSAData data;

    if (WSAStartup(0x0202, &data) != 0)
        return -1;
#endif

    return 0;
}

void LFWK_CleanupSocketLib()
{
#ifdef _MSC_VER
    WSACleanup();
#endif
}

NS_END_LFWK
