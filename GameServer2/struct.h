#pragma once

class SessionSocket;
struct IOCompletionData
{
    OVERLAPPED	    overlapped;
    char		    buffer[255];
    int			    bufferSize;
    WSABUF		    wsabuf;
    SessionSocket*  pSessionSocket;

    IOCompletionData(SessionSocket* _pSessionSocket)
        : pSessionSocket(_pSessionSocket)
    {
        bufferSize = 255;
        wsabuf.buf = buffer;
        wsabuf.len = bufferSize;
        ZeroMemory(&overlapped, sizeof(overlapped));
    }
};
