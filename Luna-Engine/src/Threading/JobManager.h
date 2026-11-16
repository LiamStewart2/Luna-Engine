#pragma once

#include <array>
#include <thread>

#include "Kernel/ConcurrentQueue.h"
#include "Job.h"

namespace Luna
{
	class JobManager
	{
	public:
		JobManager();
		~JobManager();

		ConcurrentQueue<Workload>* GetJobQueue() {return &m_JobQueue;}
		void ShutdownThreads(bool immediate); // ToDo: if immediate is true, will termianate a thread mid job - given that the job is safe to terminate on
		unsigned int GetThreadLimit() {return c_ThreadLimit;}
		Workload** GetCurrentJobs() {return m_CurrentJobs.data();}
	private:
		void InitializeThreads();
		void WorkerThreadProcess(int threadIndex);

		static constexpr unsigned int c_ThreadLimit = 8;

		bool m_Running = false;
		ConcurrentQueue<Workload> m_JobQueue;
		std::array<std::thread, c_ThreadLimit> m_ThreadPool;
		std::array<Workload*, c_ThreadLimit> m_CurrentJobs;
	};
}