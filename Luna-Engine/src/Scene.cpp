#include "Scene.h"

Scene::Scene() : lightManager(LightManager(&camera))
{

}

Scene::~Scene()
{
}

void Scene::Init(Window* _window)
{
	window = _window;
	LoadAssets();

	shader.BindShader();
	shader.SetInt("diffuseTexture", 0);
	shader.SetInt("shadowMap", 1);

	light.BuildLight(&lightManager);
	
	unsigned int sceneObject = 0;
	ECS.AddComponent<NameComponent>(sceneObject, "Scene");
	ECS.AddComponent<Transform>(sceneObject, glm::vec3(0, 0, 0));
	sceneGraph = SceneGraph(sceneObject);
	gameObjects = std::vector<unsigned int>({0});

	unsigned int plane = 1;
	ECS.AddComponent<NameComponent>(plane, "Plane");
	ECS.AddComponent<Transform>(plane, glm::vec3(0, -2, 0), glm::vec3(0, 0, 0), glm::vec3(5, 1, 5));
	ECS.AddComponent<MeshComponent>(plane, &planeMesh, &shader, &material, &defaultTexture);
	sceneGraph.InsertNode(plane);
	gameObjects.push_back(1);

	AddObject();
}

void Scene::LoadAssets()
{
	//Load all assets for the scene
	shader = Shader("Assets/Shaders/Shader/shader.vs", "Assets/Shaders/Shader/shader.fs");
	depthmapShader = Shader("Assets/Shaders/DepthShader/shader.vs", "Assets/Shaders/DepthShader/shader.fs", "Assets/Shaders/DepthShader/shader.gs");

	AssetLoader::LoadMeshOBJ(monkeyMesh, "Assets/Models/monkey.obj");
	AssetLoader::LoadMeshOBJ(planeMesh, "Assets/Models/planeobj.obj");

	AssetLoader::LoadTexture(stoneTexture, "Assets/Textures/rock.png");
	AssetLoader::LoadTexture(defaultTexture, "Assets/Textures/default.png");
	
	material = { glm::vec3(1) };
}

void Scene::Update()
{
	shader.BindShader();
	if(window->GetMouseButton(GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
		shader.SetInt("debugMode", 1);
	else
		shader.SetInt("debugMode", 0);

	light.position = glm::vec3(1, -7, 0);
	light.direction = -glm::normalize(light.position);
	camera.HandleInput(window);

	transformationManager.UpdateTransformationMatricies(&sceneGraph, &ECS);
}

void Scene::Render(Renderer* renderer)
{
	std::unordered_map<unsigned int, Transform*> transforms = ECS.GetAllComponentsOfType<Transform>();
	std::unordered_map<unsigned int, MeshComponent*> meshComponents = ECS.GetAllComponentsOfType<MeshComponent>();

	light.FrameSetup(&lightManager, &depthmapShader, &shader);
	for (auto& [id, meshComponent] : meshComponents)
	{
		auto transformIt = transforms.find(meshComponent->gameObject);
		if (transformIt != transforms.end())
			light.RenderObjectToDepthmap(meshComponent->mesh, transformIt->second, &depthmapShader);
	}
	light.FrameReset();
	
	renderer->SetShaderFrame(&camera, &depthmapShader, &shader, &light);
	for (auto& [id, meshComponent] : meshComponents)
	{
		auto transformIt = transforms.find(meshComponent->gameObject);
		if (transformIt != transforms.end())
		{
			renderer->RenderObject(transformIt->second, meshComponent->mesh, meshComponent->texture, meshComponent->material, meshComponent->shader);
		}
	}
}

void Scene::AddObject(unsigned int parent, std::string name, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale)
{
	unsigned int object = gameObjects[gameObjects.size() - 1] + 1;
	ECS.AddComponent<NameComponent>(object, name);
	ECS.AddComponent<Transform>(object, position, rotation, scale);
	ECS.AddComponent<MeshComponent>(object, &monkeyMesh, &shader, &material, &stoneTexture);

	sceneGraph.InsertNode(object, sceneGraph.GetNode(parent, nullptr));
	gameObjects.push_back(object);
}


void Scene::DestroyScene()
{
	shader.DestroyShader();
}