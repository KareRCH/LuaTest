#include "ThreadPool.h"

CThreadPool::CThreadPool(size_t num_threads)
	: m_iThreadCount(num_threads), m_bStop_All(false)
{
	m_vWorkerThread.reserve(num_threads);

	// ������ �����, �����Լ��� Main�Լ� ���α�
	for (size_t i = 0; i < num_threads; i++)
	{
		m_vWorkerThread.emplace_back([this]() { this->WorkerThread(); });
	}
}

CThreadPool::~CThreadPool()
{
	m_bStop_All = true;
	m_cvJob_Queue.notify_all();

	// ��� �����尡 ���� ������ ���
	for (auto& t : m_vWorkerThread)
	{
		t.join();
	}
}

void CThreadPool::EnqueueJob(function<void()> job)
{
	if (m_bStop_All)
	{
		throw runtime_error("ThreadPool ��� ������");
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

		// ���� �ְų� ������ ������ �س��� �� ����ϵ��� �Ѵ�.
		m_cvJob_Queue.wait(lock, [this]() { return !this->m_qJob.empty() || m_bStop_All; });
		if (m_bStop_All && this->m_qJob.empty())
		{
			return;
		}

		// �� ���� job�� �A��
		function<void()> job = move(m_qJob.front());
		m_qJob.pop();
		lock.unlock();

		// �� ť���� ���� ���� �����Ѵ�.
		job();
	}
}
