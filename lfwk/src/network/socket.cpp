#include "network/socket.h"
#include "socket_api.h"

NS_BEGIN_LFWK

Socket::Socket(LFWK_SocketHandle handle)
{
    handle_ = handle;
    LFWK_InitSockAddrIn(local_addr_, "", 0);
    LFWK_InitSockAddrIn(remote_addr_, "", 0);
    block_ = true;
    connected_ = false;
}

Socket::~Socket()
{
    Close();
}

int Socket::SetBlock(bool block)
{
    int result = 0;

    if (block) result = LFWK_SetBlock(handle_);
    else result = LFWK_SetNonBlock(handle_);

    if (result == 0)
    {
        block_ = block;
    }

    return result;
}

int Socket::SetReUseAddr()
{
    return LFWK_SetReUseAddr(handle_);
}

void Socket::Close()
{
    if (handle_ == LFWK_INVALID_SOCKET)
        return;

    if (connected_)
    {
        HandleEv_Disconnect();
        connected_ = false;
    }

    LFWK_CloseSocket(handle_);

    handle_ = LFWK_INVALID_SOCKET;
}

int Socket::GetRecvBufSize(int *size)
{
    return LFWK_GetRecvBufSize(handle_, size);
}

int Socket::SetRecvBufSize(int size)
{
    return LFWK_SetRecvBufSize(handle_, size);
}

int Socket::GetSendBufSize(int *size)
{
    return LFWK_GetSendBufSize(handle_, size);
}

int Socket::SetSendBufSize(int size)
{
    return LFWK_SetSendBufSize(handle_, size);
}

int Socket::Bind(const char *host, int port)
{
    return LFWK_Bind(handle_, host, port, &local_addr_);
}

int Socket::Listen(int backlog /* = 5 */)
{
    return LFWK_Listen(handle_, backlog);
}

int Socket::Connect(const char *host, int port)
{
    if (LFWK_Connect(handle_, host, port) != 0)
        return -1;

    connected_ = true;
    HandleEv_Connected();

    return 0;
}

int Socket::Recv(char *buf, int len, int flags /* = 0 */)
{
    int numOfBytes = ::recv(handle_, buf, len, flags);

    if (numOfBytes == 0)
    {
        Close();
        return -1;
    }

    if (numOfBytes == SOCKET_ERROR)
    {
        int errNo = 0;
#ifdef _MSC_VER
        errNo = ::WSAGetLastError();
#else
        errNo = errno;
#endif

        if (!block_)
        {
#ifdef _MSC_VER
            if (errNo != WSAEWOULDBLOCK)
#else
            if (errNo != EWOULDBLOCK && errNo != EAGAIN)
#endif
            {
                HandleEv_Error(errNo);
                return -1;
            }
            else
            {
                return 0; // zero bytes received
            }
        }
    }

    return numOfBytes; // number of bytes received
}

int Socket::Send(char *buf, int len, int flags /* = 0 */)
{
    int totalBytes = 0;

    while (len > 0)
    {
        int numOfBytes = ::send(handle_, buf, len, flags);

        if (numOfBytes > 0)
        {
            totalBytes += numOfBytes;
            buf += numOfBytes;
            len -= numOfBytes;
            continue;
        }

        if (numOfBytes == SOCKET_ERROR)
        {
            int errNo = 0;
#ifdef _MSC_VER
            errNo = ::WSAGetLastError();
#else
            errNo = errno;
#endif
            if (!block_)
            {
#ifdef _MSC_VER
                if (errNo != WSAEWOULDBLOCK)
#else
                if (errNo != EWOULDBLOCK && errNo != EAGAIN)
#endif
                {
                    HandleEv_Error(errNo);
                    return -1;
                }
            }
            else
            {
                HandleEv_Error(errNo);
                return -1;
            }

            break; // if would block, return totalBytes
        }
    }

    return totalBytes;
}

void Socket::HandleEv_Connected()
{
    OnConnected();
}

void Socket::HandleEv_Disconnect()
{
    OnDisconnect();
}

void Socket::HandleEv_Error(int errorCode)
{
    OnError(errorCode);
    Close();
}

NS_END_LFWK
