#pragma once

#include <vector>

#include "ImGuiPanel.h"

#include "Luna.h"
#include "Threading/Job.h"
#include "Threading/JobManager.h"

class JobViewPanel : public ImGuiPanel
{
public:
	JobViewPanel(SceneManager* sceneManager = nullptr, Luna::JobManager* jobManager = nullptr) : ImGuiPanel(sceneManager), m_JobManager(jobManager)
	{
		if(sceneManager == nullptr) return;
		if(m_JobManager == nullptr) return;
	}

	void Update(unsigned int& inspectorID) override;
private:
	static void IterateOverQueue(Luna::Workload* workload, int jobIndex);
	Luna::JobManager* m_JobManager = nullptr;
};