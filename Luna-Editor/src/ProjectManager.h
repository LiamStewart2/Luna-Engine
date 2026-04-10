#pragma once

#include <string>

#include "Luna.h"
#include "JSON/json.hpp"

// An instance of a project, acting as a working directory
struct Project
{
	std::string m_WorkingDirectory;
	std::string m_ProjectName;
	std::string m_DefaultScenePath;
};

class ProjectManager
{
public:
	void OpenProject(std::string filepath);
	void OnEditorStart();
	void OpenDefaultScene(SceneManager* sceneManager);

	const Project& GetOpenProject() {return m_WorkingProject;}
private:
	Project m_WorkingProject;
};

