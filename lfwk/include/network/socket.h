#pragma once
#include "def.h"

NS_BEGIN_LFWK

class Socket
{
public:
    Socket(LFWK_SocketHandle handle);
    virtual ~Socket();

public:
    LFWK_SocketHandle GetHandle() { return handle_; }

    void GetLocalAddr(LFWK_SockAddr *addr) { *addr = local_addr_; }
    void GetRemoteAddr(LFWK_SockAddr *addr) { *addr = remote_addr_; }
    void SetRemoteAddr(const LFWK_SockAddr *addr) { remote_addr_ = *addr; }

    bool IsBlock() { return block_; }
    int SetBlock(bool block);

    int SetReUseAddr();

    bool IsConnected() { return connected_; }

public:
    void Close();

    /**
     * 获取接收缓冲区大小，返回0表示成功，其他表示失败
     */
    int GetRecvBufSize(int *size);
    /**
     * 设置接收缓冲区大小，返回0表示成功，其他表示失败
     */
    int SetRecvBufSize(int size);

    int GetSendBufSize(int *size);
    int SetSendBufSize(int size);

    int Bind(const char *host, int port);
    int Listen(int backlog = 5);
    int Connect(const char *host, int port);

    virtual int Recv(void *buf, int len, int flags = 0);
    virtual int Send(void *buf, int len, int flags = 0);

protected:
    virtual void OnConnected() {}
    virtual void OnDisconnect() {}
    virtual void OnError(int errorCode) {}

protected:
    /**
     * 继承类通过重载该函数处理建立连接需要进行的操作
     */
    virtual void HandleEv_Connected();

    /**
     * 继承类通过重载该函数处理断开连接需要进行的操作
     */
    virtual void HandleEv_Disconnect();

    /**
     * 继承类通过重载该函数处理连接发生错误时需要进行的操作
     */
    virtual void HandleEv_Error(int errorCode);

protected:
    LFWK_SocketHandle handle_;
    LFWK_SockAddr local_addr_;
    LFWK_SockAddr remote_addr_;
    bool block_;
    bool connected_;
};

NS_END_LFWK
