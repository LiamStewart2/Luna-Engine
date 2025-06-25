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

	for (int x = -5; x < 5; x++)
	{
		for (int y = -5; y < 5; y++)
		{
			for (int z = -5; z < 5; z++)
			{
				objectBuffer.Push(GameObject({x * 3, y * 3, z * 3}));
				objectBuffer[objectBuffer.Size() - 1].AddComponent<CoolComponent>();
				objectBuffer[objectBuffer.Size() - 1].AddComponent<MeshRenderer>(&monkeyMesh, &shrekTexture, &material, &shader);
			}
		}
	}

	for (size_t i = 0; i < objectBuffer.Size(); i++)
		objectBuffer[i].OnStart();
}

void Scene::LoadAssets()
{
	//Load all assets for the scene
	shader = Shader("Assets/Shaders/Shader/shader.vs", "Assets/Shaders/Shader/shader.fs");

	AssetLoader::LoadMeshOBJ(monkeyMesh, "Assets/Models/monkeysmoothed.obj");

	AssetLoader::LoadTexture(shrekTexture, "Assets/Textures/rock.png");

	light = { glm::vec3(0), glm::vec3(1) };
	material = { glm::vec3(1) };
}

void Scene::Update()
{
	camera.HandleInput(window);
	light.position = glm::vec3(cos(glfwGetTime()) * 5, 0, sin(glfwGetTime()) * 5);
	
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