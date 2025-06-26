#pragma once

#include <vector>

#include "ECS/ECS.h"
#include "LinkedList.h"
#include "AssetLoader.h"
#include "Mesh.h"
#include "AssetLoader.h"
#include "Renderer.h"

class Scene
{
public:
	Scene();
	~Scene();

	void Init(GLFWwindow* window);
	void LoadAssets();

	void Update();
	void Render(Renderer* renderer);

	void DestroyScene();

	//std::vector<GameObject>* getObjectBuffer() { return &objectBuffer; }

private:
	GLFWwindow* window;

	Shader shader;
	Camera camera = Camera(glm::vec3(0, 2, -3), glm::vec3(0, 0, 0));

	Light light;
	Material material;

	Mesh planeMesh;
	Mesh monkeyMesh;

	Texture defaultTexture;
	Texture stoneTexture;

	LinkedList<GameObject> objectBuffer;
};

