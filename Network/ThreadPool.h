#pragma once
#include <condition_variable>

#define BIND_FUNC(fn) [](){ fn();} 

class ThreadPool
{
private: 
	UINT								m_threadCount;

	std::vector<std::thread>			m_threadArry;
	std::condition_variable				m_conditionalVariable;

	std::queue<std::function<void()>>	m_jobQueue;
	std::mutex							m_jobQueueMutex;

	bool								m_bStopAll;

public: // default
	ThreadPool(UINT _threadCount);
	~ThreadPool();

private:
	// Logic Work 
	void ThreadWork();

public: 
	// thread pool에 work 전달
	// 실해되는 job은 스레드에 안전하지 않음
	void AddWork(std::function<void()> _delegateWork);
};

