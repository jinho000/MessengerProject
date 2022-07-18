#pragma once
#include "SessionSocket.h"

class TCPSession
{
private: // member var
	SessionSocket			m_sessionSocket;
	IOCompletionData		m_IOCompletionData;
	IOCompletionCallback	m_IOCompletionCallback;

public: // default
	TCPSession();
	~TCPSession();

	TCPSession(const TCPSession& _other) = delete;
	TCPSession(TCPSession&& _other) = delete;
	TCPSession& operator=(const TCPSession& _other) = delete;
	TCPSession& operator=(const TCPSession&& _other) = delete;

private:
	void IOCompletionCallback(DWORD _transferredBytes, LPOVERLAPPED _IOData);

public: // member Func
	bool IsRecycleSession();

	void RequestAsyncAccept();
	void SetClientAddress();
	void RegistIOCP();
	void RequestRecv();
};
