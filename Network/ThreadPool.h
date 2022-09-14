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
	// thread pool�� work ����
	// ���صǴ� job�� �����忡 �������� ����
	void AddWork(std::function<void()> _delegateWork);
};

