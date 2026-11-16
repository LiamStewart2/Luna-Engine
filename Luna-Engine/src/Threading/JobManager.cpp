#include "JobManager.h"

namespace Luna
{
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
			m_ThreadPool[i] = std::thread(&JobManager::WorkerThreadProcess, this, i);
		}
	}

	void JobManager::ShutdownThreads(bool immediate)
	{
		m_Running = false;
		for (int i = 0; i < c_ThreadLimit; i++)
		{
			m_ThreadPool[i].join();
		}
	}

	void JobManager::WorkerThreadProcess(int threadIndex)
	{
		while (m_Running)
		{
			Workload job;
			if (m_JobQueue.Pop(job))
			{
				if(job.m_Job != nullptr && job.m_Package != nullptr)
				{
					m_CurrentJobs[threadIndex] = &job;
					job.Run();
					job.Cleanup();
					m_CurrentJobs[threadIndex] = nullptr;
				}
			}
		}
	}
}