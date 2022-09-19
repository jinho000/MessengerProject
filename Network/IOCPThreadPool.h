#pragma once


// IOCP�� �̿��� ������Ǯ
class IOCPThreadPool
{
private: // member var
	std::vector<std::thread>	m_theadArray;
	HANDLE						m_IOCPHandle;
	std::atomic<UINT>			m_jobCount;

public: // default
	IOCPThreadPool(UINT _threadCount);
	~IOCPThreadPool();

private:
	void ThreadWork();
	void Stop();

public: 
	void AddWork(std::function<void()> _work);
};
