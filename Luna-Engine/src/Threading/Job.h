#pragma once

namespace Luna
{
	struct JobPackage
	{

	};

	class Job
	{
	public:
		virtual void Work(JobPackage* package) = 0;
		bool m_SafeToShutdown = false; // if false, Shutdown will wait for this job to finish - file serialization
	};

	struct Workload
	{
		Job* m_Job = nullptr;
		JobPackage* m_Package = nullptr;

		void Run() {m_Job->Work(m_Package); }
	};
}