#pragma once
#include <stddef.h>
#include <stdlib.h>
#include <memory.h>
#include "def.h"

NS_BEGIN_LFWK

class DataPacket
{
public:
    DataPacket() : 
        data_(NULL),
        length_(0),
        rpos_(0),
        wpos_(0)
    {

    }

    DataPacket(char *data, size_t length) :
        data_(data),
        length_(length),
        rpos_(0),
        wpos_(0)
    {

    }

    ~DataPacket()
    {
        SAFE_FREE(data_);
        length_ = 0;
        rpos_ = 0;
        wpos_ = 0;
    }

private:
    DataPacket(const DataPacket&) {}
    DataPacket& operator = (const DataPacket&) {}

public:
    template<typename T>
    DataPacket& operator >> (T &val)
    {
        val = ReadAtom<T>();
        return *this;
    }

    DataPacket& operator >> (const char *&str)
    {
        str = ReadBasicStringPtr<char, unsigned short>();
        return *this;
    }

    template<typename T>
    DataPacket& operator << (const T &val)
    {
        WriteAtom(val);
        return *this;
    }

    DataPacket& operator << (const char *str)
    {
        WriteBasicString<char, unsigned short>(str, -1);
        return *this;
    }

    DataPacket& operator << (char *str)
    {
        WriteBasicString<char, unsigned short>(str, -1);
        return *this;
    }

    template<typename T>
    T ReadAtom()
    {
        T val;
        size_t sz = sizeof(T);
        if (Read(&val, sz) == 0)
        {
            memset(&val, 0, sz);
        }

        return val;
    }

    template<typename T>
    void WriteAtom(const T &val)
    {
        Write(&val, sizeof(T));
    }

    template<typename TC, typename TL>
    size_t ReadBasicString(TC *buf, size_t buflen)
    {
        size_t readable = GetReadableSize();
        if (readable >= sizeof(TL) + sizeof(TC))
        {
            size_t len = *((TL*)(data_ + rpos_));
            if (readable >= len * sizeof(TC) + sizeof(TL) + sizeof(TC))
            {
                if (buflen == 0) return len; // the length of buffer needed

                size_t readlen = len;
                if (readlen >= buflen)
                    readlen = buflen - 1;

                memcpy(buf, data_ + rpos_, readlen * sizeof(TC));
                buf[readlen] = (TC)0;

                rpos_ += len * sizeof(TC) + sizeof(TL) + sizeof(TC);
                return readlen;
            }
        }

        return 0;
    }

    template<typename TC, typename TL>
    const TC *ReadBasicStringPtr()
    {
        size_t readable = GetReadableSize();
        if (readable >= sizeof(TL) + sizeof(TC))
        {
            size_t len = *((TL*)(data_ + rpos_));
            if (readable >= len * sizeof(TC) + sizeof(TL) + sizeof(TC))
            {
                const TC *str = (TC*)(data_ + rpos_ + sizeof(TL));
                rpos_ += len * sizeof(TC) + sizeof(TL) + sizeof(TC);
                return str;
            }
        }

        return NULL;
    }

    template<typename TC, typename TL>
    void WriteBasicString(const TC *str, size_t len)
    {
        if (str == NULL)
            str = "";

        if (len == ((size_t)-1))
            len = strlen(str);
        else
            len = MIN(len, strlen(str));

        WriteAtom<TL>((TL)len);
        Write(str, len * sizeof(TC));
        WriteAtom<TC>((TC)0); // the string terminal character
    }

public:
    /**
     * buf - buffer to store data.
     * sz - number of bytes to read.
     */
    size_t Read(void *buf, size_t sz)
    {
        size_t readable = GetReadableSize();
        if (sz > readable)
            return 0;

        if (sz == 0)
            return 0;

        memcpy(buf, data_ + rpos_, sz);
        rpos_ += sz;

        return sz;
    }

    void Write(const void *buf, size_t sz)
    {
        if (sz == 0)
            return;

        size_t writable = GetWritableSize();
        if (writable < sz)
        {
            Expand(length_ + sz);
        }

        memcpy(data_ + wpos_, buf, sz);
        wpos_ += sz;
    }

    size_t GetReadableSize() const
    {
        return (wpos_ > rpos_) ? (wpos_ - rpos_) : 0;
    }

    size_t GetWritableSize() const
    {
        return (length_ > wpos_) ? (length_ - wpos_) : 0;
    }

    size_t GetRpos() const
    {
        return rpos_;
    }

    void SetRpos(size_t rpos)
    {
        if (rpos > wpos_)
            return;

        rpos_ = rpos;
    }

    size_t GetWpos() const
    {
        return wpos_;
    }

    void SetWpos(size_t wpos)
    {
        if (wpos >= wpos_)
            return;

        wpos_ = wpos;
    }

    void Expand(size_t newLength)
    {
        if (newLength <= length_)
            return;

        char *newData = (char *)malloc(newLength);

        if (data_ != NULL && length_ > 0)
        {
            memcpy(newData, data_, length_);
        }

        if (data_ != NULL)
        {
            free(data_);
        }

        data_ = newData;
        length_ = newLength;
    }

private:
    char *data_;
    size_t length_;
    size_t rpos_;
    size_t wpos_;
};

NS_END_LFWK
