#include "JobManager.h"

namespace Luna
{
	std::mutex JobManager::s_lock = std::mutex();

	JobManager::JobManager()
	{
		InitializeThreads();
	}
	JobManager::~JobManager()
	{
		ShutdownThreads(true);
	}

	void JobManager::InitializeThreads()
	{
		m_Running = true;
		for (int i = 0; i < c_ThreadLimit; i++)
		{
			m_ThreadPool[i] = std::thread(&JobManager::WorkerThreadProcess, this);
		}
	}

	void JobManager::ShutdownThreads(bool immediate)
	{
		if (immediate)
		{
			for (int i = 0; i < c_ThreadLimit; i++)
			{

			}
		}
		m_Running = false;
	}

	void JobManager::WorkerThreadProcess()
	{
		while (m_Running)
		{
			s_lock.lock();
			
			if (!m_JobQueue.isEmpty())
			{
				Workload job = m_JobQueue.Pop();
				job.Run();
			}

			s_lock.unlock();
		}
	}
}