#include "Scene.h"

Scene::Scene()
{
}

Scene::~Scene()
{
}

void Scene::Init(GLFWwindow* _window)
{
	window = _window;
	LoadAssets();


	objectBuffer.Push(GameObject({0, 2, 0}, {1, 1, 1}, {0, 180, 0}));
	objectBuffer[objectBuffer.Size() - 1].AddComponent<MeshRenderer>(&monkeyMesh, &stoneTexture, &material, &shader);

	objectBuffer.Push(GameObject({ 0, 0, 0 }, {10, 1, 10}, {0, 0, 0}));
	objectBuffer[objectBuffer.Size() - 1].AddComponent<MeshRenderer>(&planeMesh, &defaultTexture, &material, &shader);


	for (size_t i = 0; i < objectBuffer.Size(); i++)
		objectBuffer[i].OnStart();
}

void Scene::LoadAssets()
{
	//Load all assets for the scene
	shader = Shader("Assets/Shaders/Shader/shader.vs", "Assets/Shaders/Shader/shader.fs");

	AssetLoader::LoadMeshOBJ(monkeyMesh, "Assets/Models/monkeysmoothed.obj");
	AssetLoader::LoadMeshOBJ(planeMesh, "Assets/Models/planeobj.obj");

	AssetLoader::LoadTexture(stoneTexture, "Assets/Textures/rock.png");
	AssetLoader::LoadTexture(defaultTexture, "Assets/Textures/default.png");

	light = { glm::vec3(100, 100, 100), glm::vec3(1) };
	material = { glm::vec3(1) };
}

void Scene::Update()
{
	camera.HandleInput(window);
	
	for(size_t i = 0; i < objectBuffer.Size(); i++)
		objectBuffer[i].Update();
}

void Scene::Render(Renderer* renderer)
{
	renderer->SetShaderFrame(&camera, &shader, &light);
	for (size_t i = 0; i < objectBuffer.Size(); i++)
		objectBuffer[i].OnRender(renderer);
}


void Scene::DestroyScene()
{
	shader.DestroyShader();
}