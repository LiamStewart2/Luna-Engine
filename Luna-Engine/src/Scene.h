#pragma once

#include <vector>

#include "ECS/ECS.h"
#include "LinkedList.h"
#include "AssetLoader.h"
#include "Mesh.h"
#include "AssetLoader.h"
#include "Renderer.h"
#include "LightManager.h"

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

	LinkedList<GameObject>* getObjectBuffer() { return &objectBuffer; }

private:
	GLFWwindow* window;

	LinkedList<GameObject> objectBuffer;
	LightManager lightManager;

	Shader shader;
	Shader depthmapShader;

	Camera camera = Camera(glm::vec3(0, 2, -3), glm::vec3(0, 0, 0));

	Light light = Light(glm::vec3(0, 10, 10), glm::vec3(0, 1, 1), glm::vec3(1));
	Material material;

	Mesh planeMesh;
	Mesh monkeyMesh;

	Texture defaultTexture;
	Texture stoneTexture;

};

