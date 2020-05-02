#pragma once

/*
* os header file
*/
#ifdef _MSC_VER
#define WIN32_LEAN_AND_MEAN
#include <crtdbg.h>
#include <Windows.h>
#include <WinSock2.h>
//#include <MSWSock.h>
//#pragma comment(lib, "WS2_32.lib")
//#pragma comment(lib, "Mswsock.lib")
#include <mmsystem.h>
//#pragma  comment(lib, "winmm.lib")
#else
#include <netinet/in.h>
#include <stdint.h>
#include <pthread.h>
#endif

/*
* data type
*/
#ifdef _MSC_VER
typedef signed __int8  int8;
typedef signed __int16 int16;
typedef signed __int32 int32;
typedef signed __int64 int64;
typedef unsigned __int8  uint8;
typedef unsigned __int16 uint16;
typedef unsigned __int32 uint32;
typedef unsigned __int64 uint64;

typedef HANDLE      LFWK_Handle;
typedef SOCKET      LFWK_SocketHandle;
typedef HANDLE      LFWK_ThreadHandle;
typedef DWORD       LFWK_ThreadID;
typedef sockaddr_in LFWK_SockAddr;
typedef int         LFWK_SockLenType;

#define LFWK_INVALID_HANDLE ((::HANDLE)(LONG_PTR)-1)
#define LFWK_INVALID_SOCKET INVALID_SOCKET
#else
typedef int8_t  int8;
typedef int16_t int16;
typedef int32_t int32;
typedef int64_t int64;
typedef uint8_t  uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;

typedef int         LFWK_Handle;
typedef int         LFWK_SocketHandle;
typedef int         LFWK_ThreadHandle;
typedef int         LFWK_ThreadID;
typedef sockaddr_in LFWK_SockAddr;
typedef socklen_t   LFWK_SockLenType;

#define SOCKET_ERROR (-1)
#define LFWK_INVALID_HANDLE (-1)
#define LFWK_INVALID_SOCKET (-1)
#endif

/*
* namespace
*/
#define NS_BEGIN_LFWK namespace lfwk {
#define NS_END_LFWK }
#define LFWK lfwk::

/*
* stdcall
*/
#ifndef STDCALL
#ifdef _MSC_VER
#define STDCALL __stdcall
#else
#define STDCALL
#endif
#endif

/*
* assert
*/
#ifdef _DEBUG
#include <assert.h>
#define ASSERT assert
#else
#define ASSERT
#endif

#define STATIC_ASSERT(exp) typedef char UnName##__LINE__[(exp) ? 1 : -1];

#define ArrayCount(a) (sizeof(a) / sizeof((a)[0]))

#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

#define SAFE_FREE(ptr)   \
    if ((ptr) != NULL) { \
        free((ptr));     \
        (ptr) = NULL;    \
    }

#define SAFE_DELETE(ptr) \
    if ((ptr) != NULL) { \
        delete (ptr);    \
        (ptr) = NULL;    \
    }

#define SAFE_DELETE_ARRAY(ptr) \
    if ((ptr) != NULL) {       \
        delete[] (ptr);        \
        (ptr) = NULL;          \
    }

// itoa
#ifdef _MSC_VER
#define itoa _itoa
#else
#define itoa(n, s, x) sprintf(s, "%d", n)
#endif
