#pragma once

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
private:
	Scene* m_Scene = nullptr;
};


