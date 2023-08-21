#pragma once

#include <queue>
#include <functional>
#include <condition_variable>

using namespace std;

class CThreadPool
{
public:
	CThreadPool(size_t num_threads);
	~CThreadPool();

	void EnqueueJob(function<void()> job);

private:
	size_t					m_iThreadCount;
	vector<thread>			m_vWorkerThread;
	queue<function<void()>> m_qJob;
	condition_variable		m_cvJob_Queue;
	mutex					m_mxJob_Queue;

	bool m_bStop_All;

	/// <summary>
	/// 쓰레드의 Main함수
	/// </summary>
	void WorkerThread();
};

