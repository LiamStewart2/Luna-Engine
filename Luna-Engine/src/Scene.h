#pragma once

#include <vector>

#include "ECS/ECS.h"
#include "Mesh.h"
#include "AssetLoader.h"

class Scene
{
public:
	Scene();
	~Scene();

	void Init();
	void Update();

	std::vector<GameObject>* getObjectBuffer() { return &objectBuffer; }

private:
	void LoadAssets();

	std::vector<GameObject> objectBuffer;

	GameObject epicObject;
};

