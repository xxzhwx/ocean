#include "mutex.h"

NS_BEGIN_LFWK

Mutex::Mutex()
{
#ifdef _MSC_VER
    InitializeCriticalSection(&handle_);
#else
    pthread_mutex_init(&handle_, NULL);
#endif
}

Mutex::~Mutex()
{
#ifdef _MSC_VER
    DeleteCriticalSection(&handle_);
#else
    pthread_mutex_destroy(&handle_);
#endif
}

void Mutex::Lock()
{
#ifdef _MSC_VER
    EnterCriticalSection(&handle_);
#else
    pthread_mutex_lock(&handle_);
#endif
}

void Mutex::Unlock()
{
#ifdef _MSC_VER
    LeaveCriticalSection(&handle_);
#else
    pthread_mutex_unlock(&handle_);
#endif
}

NS_END_LFWK
