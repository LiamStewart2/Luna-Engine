#include "Scene.h"

Scene::Scene() : lightManager(LightManager(&camera))
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

	light.BuildLight(&lightManager);
	
	unsigned int gameObject = 1;
	ECS.AddComponent<Transform>(gameObject, glm::vec3(5, 0, 0));
	ECS.AddComponent<MeshComponent>(gameObject, &monkeyMesh, &shader, &material, &stoneTexture);
}

void Scene::LoadAssets()
{
	//Load all assets for the scene
	shader = Shader("Assets/Shaders/Shader/shader.vs", "Assets/Shaders/Shader/shader.fs");
	depthmapShader = Shader("Assets/Shaders/DepthShader/shader.vs", "Assets/Shaders/DepthShader/shader.fs", "Assets/Shaders/DepthShader/shader.gs");

	AssetLoader::LoadMeshOBJ(monkeyMesh, "Assets/Models/monkeysmoothed.obj");
	AssetLoader::LoadMeshOBJ(planeMesh, "Assets/Models/planeobj.obj");

	AssetLoader::LoadTexture(stoneTexture, "Assets/Textures/rock.png");
	AssetLoader::LoadTexture(defaultTexture, "Assets/Textures/default.png");
	
	material = { glm::vec3(1) };
}

void Scene::Update()
{
	shader.BindShader();
	if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
		shader.SetInt("debugMode", 1);
	else
		shader.SetInt("debugMode", 0);

	light.position = glm::vec3(5, -5, 0);
	light.direction = -glm::normalize(light.position);
	camera.HandleInput(window);
}

void Scene::Render(Renderer* renderer)
{

}


void Scene::DestroyScene()
{
	shader.DestroyShader();
}