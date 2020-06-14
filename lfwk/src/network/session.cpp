#include "network/session.h"

NS_BEGIN_LFWK

Session::Session() :
    socket_(NULL)
{

}

Session::~Session()
{

}

void Session::InitSocket(LFWK_SocketHandle handle)
{
    ASSERT(socket_ == NULL);

    socket_ = new Socket(handle);
    socket_->SetBlock(false);
}

void Session::OnRun()
{

}

DataPacket* Session::AllocPacket(uint8 sysId, uint8 cmd)
{
    return NULL;
}

void Session::FlushPacket(DataPacket* packet)
{

}

void Session::RecvFromSocket()
{

}

void Session::SendToSocket()
{
    size_t sendQueueSize = send_queue_.GetSize();
    if (sendQueueSize > 512)
    {
        printf("too many packet waiting to be sent: %zu\n", sendQueueSize);
    }

    DataPacket *packet;
    while (send_queue_.Peek(packet))
    {
        int sentBytes = 0;
        size_t readableSize = packet->GetReadableSize();
        if (readableSize > 0)
        {
            sentBytes = socket_->Send(packet->GetReadPtr(), (int)readableSize);

            if (sentBytes <= 0)
            {
            }
        }
    }
}

NS_END_LFWK
