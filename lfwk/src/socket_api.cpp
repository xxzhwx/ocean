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

int LFWK_CloseSocket(LFWK_SocketHandle handle)
{
#ifdef _MSC_VER
    return ::closesocket(handle);
#else
    return ::close(handle);
#endif
}

void LFWK_InitSockAddrIn(LFWK_SockAddr &addr, const char *host, unsigned short port)
{
    memset(&addr, 0, sizeof(addr));

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(host);
    addr.sin_port = htons(port);
}

LFWK_SocketHandle LFWK_Socket(int af /* = AF_INET */, int type /* = SOCK_STREAM */, int protocol /* = IPPROTO_TCP */)
{
    return ::socket(af, type, protocol);
}

LFWK_SocketHandle LFWK_CreateTcpSocket()
{
    return ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
}

int LFWK_SetBlock(LFWK_SocketHandle handle)
{
#ifdef _MSC_VER
    unsigned long flag = 0;

    if (ioctlsocket(handle, FIONBIO, &flag) == SOCKET_ERROR)
    {
        return -1;
    }
#else
    int flags = fcntl(handle_, F_GETFL, 0);
    if (flags == -1)
    {
        return -1;
    }

    flags &= ~O_NONBLOCK;

    if (fcntl(handle_, F_SETFL, flags) == -1)
    {
        return -1;
    }
#endif

    return 0;
}

int LFWK_SetNonBlock(LFWK_SocketHandle handle)
{
#ifdef _MSC_VER
    unsigned long flag = 1;

    if (ioctlsocket(handle, FIONBIO, &flag) == SOCKET_ERROR)
    {
        return -1;
    }
#else
    int flags = fcntl(handle_, F_GETFL, 0);
    if (flags == -1)
    {
        return -1;
    }

    flags |= O_NONBLOCK;

    if (fcntl(handle_, F_SETFL, flags) == -1)
    {
        return -1;
    }
#endif

    return 0;
}

int LFWK_SetReUseAddr(LFWK_SocketHandle handle)
{
    int flag = 1;
    if (::setsockopt(handle, SOL_SOCKET, SO_REUSEADDR, (const char*)&flag, sizeof(flag)) == SOCKET_ERROR)
    {
        return -1;
    }

    return 0;
}

int LFWK_GetRecvBufSize(LFWK_SocketHandle handle, int *size)
{
    int oplen = sizeof(*size);
    if (::getsockopt(handle, SOL_SOCKET, SO_RCVBUF, (char*)size, &oplen) == SOCKET_ERROR)
    {
        return -1;
    }

    return 0;
}

int LFWK_SetRecvBufSize(LFWK_SocketHandle handle, int size)
{
    if (::setsockopt(handle, SOL_SOCKET, SO_RCVBUF, (const char*)&size, sizeof(size)) == SOCKET_ERROR)
    {
        return -1;
    }

    return 0;
}

int LFWK_GetSendBufSize(LFWK_SocketHandle handle, int *size)
{
    int oplen = sizeof(*size);
    if (::getsockopt(handle, SOL_SOCKET, SO_SNDBUF, (char*)size, &oplen) == SOCKET_ERROR)
    {
        return -1;
    }

    return 0;
}

int LFWK_SetSendBufSize(LFWK_SocketHandle handle, int size)
{
    if (::setsockopt(handle, SOL_SOCKET, SO_SNDBUF, (const char*)&size, sizeof(size)) == SOCKET_ERROR)
    {
        return -1;
    }

    return 0;
}

int LFWK_Bind(LFWK_SocketHandle handle, const char *host, unsigned short port, LFWK_SockAddr *localAddr /*= NULL*/)
{
    LFWK_SockAddr addr;
    LFWK_InitSockAddrIn(addr, host, port);

    if (::bind(handle, (const sockaddr*)&addr, sizeof(addr)) == SOCKET_ERROR)
    {
        return -1;
    }

    if (localAddr != NULL)
    {
        *localAddr = addr;
    }

    return 0;
}

int LFWK_Listen(LFWK_SocketHandle handle, int backlog /* = 5 */)
{
    if (::listen(handle, backlog) == SOCKET_ERROR)
    {
        return -1;
    }

    return 0;
}

LFWK_SocketHandle LFWK_Accept(LFWK_SocketHandle handle, LFWK_SockAddr *addr)
{
    int addrlen = sizeof(*addr);
    return ::accept(handle, (sockaddr*)addr, &addrlen);
}

int LFWK_Connect(LFWK_SocketHandle handle, const char *host, unsigned short port)
{
    LFWK_SockAddr addr;
    LFWK_InitSockAddrIn(addr, host, port);

    if (::connect(handle, (const sockaddr*)&addr, sizeof(addr)) == SOCKET_ERROR)
    {
        return -1;
    }

    return 0;
}

NS_END_LFWK
