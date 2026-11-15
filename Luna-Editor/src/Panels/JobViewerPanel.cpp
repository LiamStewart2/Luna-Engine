#include "JobViewerPanel.h"

void JobViewPanel::Update(unsigned int& inspectorID)
{
	ImGui::Begin("Job View");

	// every frame, make a copy of the job queue to display

	const std::string* wopJobNames = m_JobManager->GetCurrentJobs();

	bool unemployed = true;
	for(int i = 0; i < m_JobManager->GetThreadLimit(); i++)
	{
		if(wopJobNames[i] != "")
		{
			ImGui::TextColored(ImVec4(0, 255, 0, 255), wopJobNames[i].c_str());
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
	ImGui::Text(workload->m_Job->m_JobName);
}