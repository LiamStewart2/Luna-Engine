#include "JobViewerPanel.h"

void JobViewPanel::Update(unsigned int& inspectorID)
{
	ImGui::Begin("Job View");

	// every frame, make a copy of the job queue to display

	Luna::Workload** wopJobNames = m_JobManager->GetCurrentJobs();

	bool unemployed = true;
	for(int i = 0; i < m_JobManager->GetThreadLimit(); i++)
	{
		if(wopJobNames[i] != nullptr)
		{
			ImGui::TextColored(ImVec4(0, 255, 0, 255), wopJobNames[i]->m_Job->JobDescription(wopJobNames[i]->m_Package).c_str());
			unemployed = false;
		}
	}
	if (unemployed)
		ImGui::Text("No jobs currently.");

	m_JobManager->GetJobQueue()->Iterate(IterateOverQueue);

	ImGui::End();
}

void JobViewPanel::IterateOverQueue(Luna::Workload* workload)
{
	ImGui::Text(workload->m_Job->JobDescription(workload->m_Package).c_str());
}