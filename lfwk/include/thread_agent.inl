
template<typename ObjectType>
ThreadAgent<ObjectType>::ThreadAgent() :
    _handle(LFWK_INVALID_HANDLE)
{
}

template<typename ObjectType>
ThreadAgent<ObjectType>::~ThreadAgent()
{
    LFWK_CloseThreadHandle(_handle);
}

template<typename ObjectType>
void ThreadAgent<ObjectType>::Initialize(ObjectType *object, Function function)
{
    _object = object;
    _function = function;
}

template<typename ObjectType>
void ThreadAgent<ObjectType>::Activate()
{
    _handle = LFWK_CreateThread(Wrapper, this);
}

template<typename ObjectType>
void ThreadAgent<ObjectType>::Wait()
{
    LFWK_WaitThread(_handle);
}

template<typename ObjectType>
#ifdef _MSC_VER
unsigned long WINAPI
#else
void*
#endif
ThreadAgent<ObjectType>::Wrapper(void* param)
{
    ThreadAgent* agent = (ThreadAgent*)param;
    ObjectType* object = agent->_object;
    Function function = agent->_function;
    (object->*function)();

    return NULL;
}
