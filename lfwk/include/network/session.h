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
    void SetRecvSize(size_t size) { recv_size_ = size; }

    DataPacket* AllocPacket(uint32 opcode);
    void FlushPacket(DataPacket* packet);

    virtual void OnRun();

    void RecvFromSocket();
    void SendToSocket();

private:
    void OnRecv(); //从接收缓冲区接收完整的包并处理

protected:
    virtual void OnHandlePacket(uint32 opcode, char* buf, size_t size);

private:
    DataPacket *recv_buf_;
    size_t recv_size_; //每次从套接字读取的最大字节数
    SafeQueue<DataPacket*> send_queue_;

    Socket *socket_;
};

NS_END_LFWK
