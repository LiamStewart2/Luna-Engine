#pragma once

#include <array>
#include <thread>
#include <mutex>

#include "Core/ConcurrentQueue.h"
#include "Job.h"

namespace Luna
{
	class JobManager
	{
	public:
		JobManager();
		~JobManager();

		ConcurrentQueue<Workload>* GetJobQueue() {return &m_JobQueue;}
		void ShutdownThreads(bool immediate); // if immediate is true, will termianate a thread mid job - given that the job is safe to terminate on
	private:
		void InitializeThreads();
		void WorkerThreadProcess();

		static constexpr unsigned int c_ThreadLimit = 8;

		bool m_Running = false;
		ConcurrentQueue<Workload> m_JobQueue;
		std::array<std::thread, c_ThreadLimit> m_ThreadPool;
		static std::mutex s_lock;
	};
}