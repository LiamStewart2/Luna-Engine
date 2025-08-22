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

	void SaveScene();

	Scene* GetCurrentScene() const { return m_Scene; }
private:
	void LoadRelations(const nlohmann::json& originalData, const nlohmann::json& jsonData, unsigned int parentObjectID);

	AssetManager assetManager;

	Material defaultMat = Material(glm::vec3(1));

	Scene* m_Scene = nullptr;
};


