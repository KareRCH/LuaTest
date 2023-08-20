#include "ThreadPool.h"

CThreadPool::CThreadPool(size_t num_threads)
	: m_iThreadCount(num_threads), m_bStop_All(false)
{
	m_vWorkerThread.reserve(num_threads);

	// 쓰레드 만들기, 람다함수로 Main함수 감싸기
	for (size_t i = 0; i < num_threads; i++)
	{
		m_vWorkerThread.emplace_back([this]() { this->WorkerThread(); });
	}
}

CThreadPool::~CThreadPool()
{
	m_bStop_All = true;
	m_cvJob_Queue.notify_all();

	// 모든 쓰레드가 끝날 때까지 대기
	for (auto& t : m_vWorkerThread)
	{
		t.join();
	}
}

void CThreadPool::EnqueueJob(function<void()> job)
{
	if (m_bStop_All)
	{
		throw runtime_error("ThreadPool 사용 중지됨");
	}
	{
		lock_guard<mutex> lock(m_mxJob_Queue);
		m_qJob.push(move(job));
	}
	m_cvJob_Queue.notify_one();
}

void CThreadPool::WorkerThread()
{
	while (true)
	{
		unique_lock<mutex> lock(m_mxJob_Queue);

		// 일이 있거나 쓰레드 정지를 해놓을 때 대기하도록 한다.
		m_cvJob_Queue.wait(lock, [this]() { return !this->m_qJob.empty() || m_bStop_All; });
		if (m_bStop_All && this->m_qJob.empty())
		{
			return;
		}

		// 맨 앞의 job을 뺸다
		function<void()> job = move(m_qJob.front());
		m_qJob.pop();
		lock.unlock();

		// 잡 큐에서 뺴온 일을 수행한다.
		job();
	}
}
