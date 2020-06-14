#pragma once
#include "def.h"
#include "mutex.h"

#include <queue>
using std::deque;

NS_BEGIN_LFWK

template<typename T>
class SafeQueue
{
public:
    SafeQueue();
    ~SafeQueue();

public:
    void Enqueue(const T &value);
    bool Dequeue(T &value);
    size_t GetSize() const;

    bool Peek(T &value);

private:
    deque<T> queue_;
    Mutex lock_;
};

template<typename T>
SafeQueue<T>::SafeQueue() {}

template<typename T>
SafeQueue<T>::~SafeQueue() {}

template<typename T>
void SafeQueue<T>::Enqueue(const T &value)
{
    lock_.Lock();
    queue_.push_back(value);
    lock_.Unlock();
}

template<typename T>
bool SafeQueue<T>::Dequeue(T &value)
{
    lock_.Lock();

    if (queue_.empty())
    {
        lock_.Unlock();
        return false;
    }

    value = queue_.front();
    queue_.pop_front();

    lock_.Unlock();
    return true;
}

template<typename T>
size_t SafeQueue<T>::GetSize() const
{
    return queue_.size();
}

template<typename T>
bool SafeQueue<T>::Peek(T &value)
{
    if (queue_.empty())
    {
        return false;
    }

    value = queue_.front();
    return true;
}

NS_END_LFWK
