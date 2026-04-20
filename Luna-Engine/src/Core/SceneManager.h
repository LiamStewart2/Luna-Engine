#pragma once

#include <fstream>

#include "JSON/json.hpp"

#include "Scene.h"

class SceneManager
{
public:
	SceneManager();
	~SceneManager();

	void Update(bool runtime = false);

	void LoadNewScene(const char* filepath);
	void UnloadCurrentScene();

	void SetAssetWorkingPath(std::string workingDir);

	void SaveScene(Scene* scene = nullptr, std::string path = "");

	void SaveSceneAs(Scene* scene, std::string path);
	void SaveSceneNode(nlohmann::json& data, SceneGraphNode* node);


	Scene* GetCurrentScene() const { return m_Scene; }
	Luna::AssetManager* GetAssetManager() { return &assetManager; }

	unsigned int AddObject(unsigned int parent = 0, std::string objectName = "");
private:
	void LoadRelations(const nlohmann::json& originalData, const nlohmann::json& jsonData, unsigned int parentObjectID);

	Luna::AssetManager assetManager;
	std::string m_WorkingDirectory;

	Scene* m_Scene = nullptr;
};


