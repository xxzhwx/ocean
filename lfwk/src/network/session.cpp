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
        size_t readableSize = packet->GetReadableSize();
        if (readableSize == 0)
        {
            DataPacket* tmp;
            send_queue_.Dequeue(tmp);
            SAFE_DELETE(tmp);

            continue;
        }

        int sentBytes = socket_->Send(packet->GetReadPtr(), (int)readableSize);

        if (sentBytes <= 0)
            break;

        if (sentBytes >= readableSize)
        {
            DataPacket* tmp;
            send_queue_.Dequeue(tmp);
            SAFE_DELETE(tmp);
        }
        else
        {
            packet->AddRpos(sentBytes);
            break;
        }
    }
}

NS_END_LFWK
