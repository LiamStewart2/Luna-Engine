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

	Scene* GetCurrentScene() const { return m_Scene; }
private:
	void LoadRelations(const nlohmann::json& jsonData);

	Scene* m_Scene = nullptr;
};


