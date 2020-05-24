#pragma once
#include "def.h"

NS_BEGIN_LFWK

int LFWK_StartupSocketLib();
void LFWK_CleanupSocketLib();

int LFWK_CloseSocket(LFWK_SocketHandle handle);
void LFWK_InitSockAddrIn(LFWK_SockAddr &addr, const char *host, unsigned short port);
LFWK_SocketHandle LFWK_Socket(int af = AF_INET, int type = SOCK_STREAM, int protocol = IPPROTO_TCP);
LFWK_SocketHandle LFWK_CreateTcpSocket();
int LFWK_SetBlock(LFWK_SocketHandle handle);
int LFWK_SetNonBlock(LFWK_SocketHandle handle);
int LFWK_SetReUseAddr(LFWK_SocketHandle handle);
int LFWK_GetRecvBufSize(LFWK_SocketHandle handle, int *size);
int LFWK_SetRecvBufSize(LFWK_SocketHandle handle, int size);
int LFWK_GetSendBufSize(LFWK_SocketHandle handle, int *size);
int LFWK_SetSendBufSize(LFWK_SocketHandle handle, int size);
int LFWK_Bind(LFWK_SocketHandle handle, const char *host, unsigned short port, LFWK_SockAddr *localAddr = NULL);
int LFWK_Listen(LFWK_SocketHandle handle, int backlog = 5);
LFWK_SocketHandle LFWK_Accept(LFWK_SocketHandle handle, LFWK_SockAddr *addr);
int LFWK_Connect(LFWK_SocketHandle handle, const char *host, unsigned short port);

NS_END_LFWK
