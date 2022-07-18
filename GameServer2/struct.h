#pragma once

class TCPSession;
struct IOCompletionData
{
    OVERLAPPED	    overlapped;
    char		    buffer[255];
    int			    bufferSize;
    WSABUF		    wsabuf;
    TCPSession&     tcpSession;

    IOCompletionData(TCPSession& _tcpSession)
        : tcpSession(_tcpSession)
    {
        bufferSize = 255;
        wsabuf.buf = buffer;
        wsabuf.len = bufferSize;
        ZeroMemory(&overlapped, sizeof(overlapped));
    }
};
