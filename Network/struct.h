#pragma once

#include <PacketLibrary/enum.h>

class TCPSession;
struct IOCompletionData
{
    enum { BUFFER_SIZE = 1024 };

    OVERLAPPED	    overlapped;
    char		    buffer[BUFFER_SIZE];
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
        wsabuf.len = BUFFER_SIZE;
    }

    void Clear()
    {
        memset(buffer, 0x00, sizeof(buffer));
        wsabuf.len = sizeof(buffer);
        wsabuf.buf = buffer;
    }



    void SetBuffer(const std::vector<uint8_t>& _sendBuffer, UINT _start, UINT _end)
    {
        Clear();

        wsabuf.buf = buffer;
        wsabuf.len = BUFFER_SIZE;
        
        // 데이터 범위 체크 
        assert(_start < _end);
        assert((_end - _start) <= BUFFER_SIZE);
        assert(_end <= _sendBuffer.size());

        std::copy(_sendBuffer.begin() + _start, _sendBuffer.begin() + _end, buffer);
    }
};
