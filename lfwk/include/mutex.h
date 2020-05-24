#pragma once
#include "def.h"

NS_BEGIN_LFWK

class Mutex
{
public:
    Mutex();
    ~Mutex();

public:
    void Lock();
    void Unlock();

private:
#ifdef _MSC_VER
    CRITICAL_SECTION handle_;
#else
    pthread_mutex_t handle_;
#endif
};

NS_END_LFWK
