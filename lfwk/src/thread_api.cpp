#include "thread_api.h"

NS_BEGIN_LFWK

LFWK_ThreadHandle LFWK_CreateThread(LFWK_ThreadFunc function, LFWK_ThreadArg arg)
{
#ifdef _MSC_VER
    DWORD id = 0;
    LFWK_ThreadHandle handle = ::CreateThread(NULL, NULL, function, arg, NULL, &id);
#else
    LFWK_ThreadHandle handle;
    ::pthread_create(&handle, NULL, function, arg);
#endif

    return handle;
}

int LFWK_GetCurrentThreadId()
{
#ifdef _MSC_VER
    return ::GetCurrentThreadId();
#else
    return ::pthread_self();
#endif
}

void LFWK_WaitThread(LFWK_ThreadHandle handle)
{
#ifdef _MSC_VER
    ::WaitForSingleObject(handle, INFINITE);
#else
    ::pthread_join(handle, NULL);
#endif
}

int LFWK_CloseThreadHandle(LFWK_ThreadHandle handle)
{
#ifdef _MSC_VER
    return ::CloseHandle(handle);
#else
    return ::close(handle);
#endif
}

void LFWK_Sleep(int milliseconds)
{
#ifdef _MSC_VER
    ::Sleep(milliseconds);
#else
    ::usleep(milliseconds * 1000);
#endif
}

NS_END_LFWK
