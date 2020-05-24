#pragma once
#include "def.h"

NS_BEGIN_LFWK

LFWK_ThreadHandle LFWK_CreateThread(LFWK_ThreadFunc function, LFWK_ThreadArg arg);
int LFWK_GetCurrentThreadId();
void LFWK_WaitThread(LFWK_ThreadHandle handle);
int LFWK_CloseThreadHandle(LFWK_ThreadHandle handle);

void LFWK_Sleep(int milliseconds);

NS_END_LFWK
