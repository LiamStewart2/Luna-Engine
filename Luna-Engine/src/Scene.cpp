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

	shader.BindShader();
	shader.SetInt("diffuseTexture", 0);
	shader.SetInt("shadowMap", 1);

	light.BuildLight();

	objectBuffer.Push(GameObject({0, 3, 0}, {1, 1, 1}, {0, 0, 0}));
	objectBuffer[objectBuffer.Size() - 1].AddComponent<MeshRenderer>(&monkeyMesh, &stoneTexture, &material, &shader);

	objectBuffer.Push(GameObject({ 2, 3, 2 }, { 1, 1, 1 }, { 0, 0, 0 }));
	objectBuffer[objectBuffer.Size() - 1].AddComponent<MeshRenderer>(&monkeyMesh, &stoneTexture, &material, &shader);
	objectBuffer[objectBuffer.Size() - 1].AddComponent<CoolComponent>();

	objectBuffer.Push(GameObject({ 0, 0, 0 }, {10, 1, 10}, {0, 0, 0}));
	objectBuffer[objectBuffer.Size() - 1].AddComponent<MeshRenderer>(&planeMesh, &defaultTexture, &material, &shader);


	for (size_t i = 0; i < objectBuffer.Size(); i++)
		objectBuffer[i].OnStart();
}

void Scene::LoadAssets()
{
	//Load all assets for the scene
	shader = Shader("Assets/Shaders/Shader/shader.vs", "Assets/Shaders/Shader/shader.fs");
	depthmapShader = Shader("Assets/Shaders/DepthShader/shader.vs", "Assets/Shaders/DepthShader/shader.fs");

	AssetLoader::LoadMeshOBJ(monkeyMesh, "Assets/Models/monkeysmoothed.obj");
	AssetLoader::LoadMeshOBJ(planeMesh, "Assets/Models/planeobj.obj");

	AssetLoader::LoadTexture(stoneTexture, "Assets/Textures/rock.png");
	AssetLoader::LoadTexture(defaultTexture, "Assets/Textures/default.png");
	
	material = { glm::vec3(1) };
}

void Scene::Update()
{
	light.position = glm::vec3(glm::sin(glfwGetTime()) * 5, 10, glm::cos(glfwGetTime()) * 5);
	light.direction = -glm::normalize(light.position);
	camera.HandleInput(window);
	
	for(size_t i = 0; i < objectBuffer.Size(); i++)
		objectBuffer[i].Update();
}

void Scene::Render(Renderer* renderer)
{
	light.FrameSetup(&depthmapShader, &shader);
	for (size_t i = 0; i < objectBuffer.Size(); i++)
		light.RenderObjectToDepthmap(objectBuffer[i].GetComponent<MeshRenderer>().get()->mesh, objectBuffer[i].GetComponent<Transform>().get(), &depthmapShader);
	light.FrameReset();
	
	renderer->SetShaderFrame(&camera, &depthmapShader, &shader, &light);
	for (size_t i = 0; i < objectBuffer.Size(); i++)
		objectBuffer[i].OnRender(renderer);
}


void Scene::DestroyScene()
{
	shader.DestroyShader();
}