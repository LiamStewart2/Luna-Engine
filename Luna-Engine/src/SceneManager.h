#pragma once

#include <fstream>

#include "JSON/json.hpp"

#include "Scene.h"

class SceneManager
{
public:
	SceneManager();
	~SceneManager();


	void Update();
	void Render(Renderer* renderer);

	void LoadNewScene(const char* filepath);
	void UnloadCurrentScene();

	void SaveCurrentScene(std::string optionalPath = "");
	void SaveSceneNode(nlohmann::json& data, SceneGraphNode* node);

	Scene* GetCurrentScene() const { return m_Scene; }
	AssetManager* GetAssetManager() { return &assetManager; }
	LightManager* GetLightManager() { return &lightManager; }

	unsigned int AddObject(unsigned int parent = 0, std::string objectName = "");
private:
	void LoadRelations(const nlohmann::json& originalData, const nlohmann::json& jsonData, unsigned int parentObjectID);

	AssetManager assetManager;
	LightManager lightManager;

	Material defaultMat = Material(glm::vec3(1));

	Scene* m_Scene = nullptr;
};


