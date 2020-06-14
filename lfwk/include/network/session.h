#pragma once
#include "def.h"
#include "datapacket.h"
#include "safe_queue.h"
#include "network/socket.h"

NS_BEGIN_LFWK

class Session
{
public:
    const static size_t MAX_PACKET_LEN = 32 * 1024 * 1024; //最大的包长度

public:
    Session();
    virtual ~Session();

    void InitSocket(LFWK_SocketHandle handle);

    virtual void OnRun();

    DataPacket* AllocPacket(uint8 sysId, uint8 cmd);
    void FlushPacket(DataPacket* packet);

    void RecvFromSocket();
    void SendToSocket();

private:
    DataPacket *recv_buf_;
    SafeQueue<DataPacket*> send_queue_;

    Socket *socket_;
};

NS_END_LFWK
