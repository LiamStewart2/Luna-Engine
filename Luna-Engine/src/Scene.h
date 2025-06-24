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

	void PushBackObject(Mesh* mesh);
	std::vector<Mesh*>* getObjectBuffer() { return &objectBuffer; }

private:
	void LoadAssets();

	std::vector<Mesh*> objectBuffer;
	
	GameObject epicObject;
};

