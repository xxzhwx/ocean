#pragma once
#include "def.h"
#include "thread_api.h"

NS_BEGIN_LFWK

template<typename ObjectType>
class ThreadAgent
{
    typedef void (ObjectType::*Function)();

public:
    ThreadAgent();
    virtual ~ThreadAgent();

public:
    virtual void Initialize(ObjectType *object, Function function);
    virtual void Activate();
    virtual void Wait();

protected:
#ifdef _MSC_VER
    static unsigned long WINAPI Wrapper(void* param);
#else
    static void* Wrapper(void* param);
#endif

protected:
    LFWK_ThreadHandle _handle;
    ObjectType* _object;
    Function _function;
};

#include "thread_agent.inl"

NS_END_LFWK
