#pragma once

#include <PacketLibrary/enum.h>

class TCPSession;
struct IOCompletionData
{
    OVERLAPPED	    overlapped;
    char		    buffer[IOBUFFER_SIZE];
    WSABUF		    wsabuf;
    IOTYPE          IOType;
    TCPSession&     tcpSession;

    IOCompletionData(TCPSession& _tcpSession, IOTYPE _IOType)
        : buffer()
        , IOType(_IOType)
        , tcpSession(_tcpSession)
    {
        ZeroMemory(&overlapped, sizeof(overlapped));
        
        wsabuf.buf = buffer;
        wsabuf.len = IOBUFFER_SIZE;
    }

    void Clear()
    {
        memset(buffer, 0x00, sizeof(buffer));
        wsabuf.len = sizeof(buffer);
        wsabuf.buf = buffer;
    }

    void SetBuffer(const std::vector<uint8_t>& _sendBuffer)
    {
        wsabuf.buf = buffer;
        wsabuf.len = static_cast<ULONG>(_sendBuffer.size());
        
        // 데이터 범위 체크 최대 255크기 까지
        assert(wsabuf.len <= IOBUFFER_SIZE);

        std::copy(_sendBuffer.begin(), _sendBuffer.end(), buffer);
    }
};
