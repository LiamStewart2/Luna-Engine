#pragma once

#include <vector>

#include "ECS/ECS.h"
#include "Mesh.h"
#include "AssetLoader.h"
#include "Renderer.h"

class Scene
{
public:
	Scene();
	~Scene();

	void Init();
	void Update();
	void Render(Renderer* renderer);

	std::vector<GameObject>* getObjectBuffer() { return &objectBuffer; }

	GameObject epicObject;
private:
	void LoadAssets();

	std::vector<GameObject> objectBuffer;

};

