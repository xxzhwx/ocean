#include "network/session.h"

NS_BEGIN_LFWK

Session::Session() :
    recv_size_(2048),
    socket_(NULL)
{
    recv_buf_ = new DataPacket();
}

Session::~Session()
{
    SAFE_DELETE(recv_buf_);
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
    int totalRecvSize = 0;

    while (true)
    {
        const size_t writableSize = recv_buf_->GetWritableSize();
        if (writableSize < recv_size_)
        {
            size_t length = recv_buf_->GetLength();
            size_t newLength = length + recv_size_ * 2;
            recv_buf_->Expand(newLength);
        }

        int recvSize = socket_->Recv(recv_buf_->GetWritePtr(), (int)recv_size_);

        if (recvSize <= 0)
            break;

        recv_buf_->AddWpos(recvSize);
        totalRecvSize += recvSize;

        //超过2M则break，尝试先处理
        if (recv_buf_->GetReadableSize() >= 2 * 1024 * 1024)
            break;
    }

    if (totalRecvSize > 0)
    {
        OnRecv();
    }
}

void Session::OnRecv()
{
    //连接已断开则丢弃剩余的数据
    if (!socket_->IsConnected())
    {
        recv_buf_->SetRpos(0);
        recv_buf_->SetWpos(0);
        return;
    }

    //没数据可处理
    if (recv_buf_->GetReadableSize() == 0)
    {
        return;
    }

    //todo
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
