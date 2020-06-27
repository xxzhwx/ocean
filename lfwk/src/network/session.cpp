#include "network/session.h"
#include "network/net_def.h"

NS_BEGIN_LFWK

Session::Session() :
    recv_size_(2048),
    socket_(nullptr)
{
    recv_buf_ = new DataPacket();
}

Session::~Session()
{
    SAFE_DELETE(recv_buf_);
}

void Session::InitSocket(LFWK_SocketHandle handle)
{
    ASSERT(socket_ == nullptr);

    socket_ = new Socket(handle);
    socket_->SetBlock(false);
}

DataPacket* Session::AllocPacket(uint32 opcode)
{
    DataPacket* packet = new DataPacket(); //todo 用对象池优化包分配和回收

    PacketHeader* hdr = (PacketHeader*)packet->GetData();
    memset(hdr, 0, sizeof(PacketHeader));
    hdr->tag = TAG_VALUE;

    packet->AddWpos(sizeof(PacketHeader));
    packet->WriteAtom(opcode);

    return packet;
}

void Session::FlushPacket(DataPacket* packet)
{
    ASSERT(packet->GetReadableSize() >= sizeof(PacketHeader));
    //向协议头写入数据长度
    PacketHeader* hdr = (PacketHeader*)packet->GetData();
    hdr->len = (uint32)(packet->GetReadableSize() - sizeof(PacketHeader));
    //放入发送队列
    send_queue_.Enqueue(packet);
}

void Session::OnRun()
{
    if (socket_ != nullptr && socket_->IsConnected())
    {
        RecvFromSocket();
        SendToSocket();
    }
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

    //处理数据
    static const size_t HEADER_SIZE = sizeof(PacketHeader);
    char* buf = recv_buf_->GetReadPtr();

    int errNo = 0;
    int lastErrNo = 0;

    size_t readableSize = recv_buf_->GetReadableSize();
    while (readableSize >= HEADER_SIZE)
    {
        PacketHeader* hdr = (PacketHeader*)buf;

        if (hdr->tag == TAG_VALUE)
        {
            size_t total = hdr->len + HEADER_SIZE;

            if (hdr->len >= sizeof(uint32)) //opcode
            {
                if (total < MAX_PACKET_LEN)
                {
                    if (readableSize < total)
                        break;

                    char* packet = (char*)(hdr + 1);
                    uint32* opcode = (uint32*)packet;
                    packet += sizeof(uint32);
                    OnHandlePacket(*opcode, packet, hdr->len - sizeof(uint32));
                }
                else
                {
                    errNo = -3; //超长
                    total = HEADER_SIZE;
                }
            }
            else
            {
                errNo = -2; //非法的长度
            }

            buf += total;
            readableSize -= total;
        }
        else
        {
            errNo = -1; //非法的TAG

            //从下一字节开始找下一个合法的TAG
            buf += 1;
            readableSize -= 1;
        }

        if (errNo != lastErrNo)
        {
            lastErrNo = errNo;
            printf("<%s> dropped a packet, errNo:%d\n", __FUNCTION__, errNo);
        }
    }

    recv_buf_->AddRpos(buf - recv_buf_->GetReadPtr());

    //把剩余的数据挪到开头
    size_t leftSize = recv_buf_->GetReadableSize();
    if (leftSize > 0)
    {
        memmove(recv_buf_->GetData(), recv_buf_->GetReadPtr(), leftSize);
    }
    recv_buf_->SetWpos(leftSize);
    recv_buf_->SetRpos(0);
}

void Session::OnHandlePacket(uint32 opcode, char* buf, size_t size)
{
    printf("on handle packet: %u, %zu\n", opcode, size);
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
