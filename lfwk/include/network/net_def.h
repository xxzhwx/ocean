#pragma once
#include "def.h"

NS_BEGIN_LFWK

#define TAG_VALUE 0xCCEE

#pragma pack(push, 4)

typedef struct
{
    uint32 tag;
    uint32 len;
    struct
    {
        uint16 dataSum;
        uint16 hdrSum;
    } check;
} PacketHeader;

#pragma pack(pop)

NS_END_LFWK
