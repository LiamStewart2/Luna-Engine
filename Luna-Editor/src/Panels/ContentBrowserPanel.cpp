#include "ContentBrowserPanel.h"

void ContentBrowserPanel::Update(unsigned int& inspectorID)
{
	ImGui::Begin("Content Browser");

	if (m_CurrentDirectory != m_ProjectDirectory)
	{
		if(ImGui::Button("<-"))
			m_CurrentDirectory = m_CurrentDirectory.parent_path();
	}

	for (auto& path : std::filesystem::directory_iterator(m_CurrentDirectory))
	{
		if(path.is_directory())
		{
			if (ImGui::Button(path.path().string().c_str()))
			{
				m_CurrentDirectory = path.path();
				break;
			}
		}
		else
		{
			ImGui::Text(path.path().string().c_str());
		}

	}

	ImGui::Text(m_ProjectDirectory.string().c_str());
	ImGui::Text(m_CurrentDirectory.string().c_str());

	ImGui::End();
}
