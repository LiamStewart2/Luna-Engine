#include "Scene.h"

Scene::Scene() : lightManager(LightManager(camera))
{

}

Scene::~Scene()
{
}

void Scene::Init(Window* _window)
{
	window = _window;
	LoadAssets();

	std::shared_ptr<Shader> shader = assetManager.GetShader("Assets/Shaders/Shader");

	shader->BindShader();
	shader->SetInt("diffuseTexture", 0);
	shader->SetInt("shadowMap", 1);

	light.BuildLight(&lightManager);
	
	unsigned int sceneObject = 0;
	ECS.AddComponent<NameComponent>(sceneObject, "Scene");
	ECS.AddComponent<Transform>(sceneObject, glm::vec3(0, 0, 0));
	sceneGraph = SceneGraph(sceneObject);
	gameObjects = std::vector<unsigned int>({0});

	unsigned int plane = 1;
	ECS.AddComponent<NameComponent>(plane, "Plane");
	ECS.AddComponent<Transform>(plane, glm::vec3(0, -2, 0), glm::vec3(0, 0, 0), glm::vec3(5, 1, 5));
	ECS.AddComponent<MeshComponent>(plane, assetManager.GetMesh("Assets/Models/planeobj.obj").get(), shader.get(), &material, assetManager.GetTexture("Assets/Textures/default.png").get());
	sceneGraph.InsertNode(plane);
	gameObjects.push_back(plane);

	unsigned int camera = 2;
	ECS.AddComponent<NameComponent>(camera, "Camera");
	ECS.AddComponent<Transform>(camera, glm::vec3(-10, 2, 0), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1));
	ECS.AddComponent<CameraComponent>(camera, new PerspectiveCamera(), true);
	sceneGraph.InsertNode(camera);
	gameObjects.push_back(camera);

	AddObject();
}

void Scene::LoadAssets()
{
	assetManager.GetShader("Assets/Shaders/Shader");
	assetManager.GetShader("Assets/Shaders/DepthShader");

	assetManager.GetMesh("Assets/Models/monkey.obj");
	assetManager.GetMesh("Assets/Models/planeobj.obj");

	assetManager.GetTexture("Assets/Textures/rock.png");
	assetManager.GetTexture("Assets/Textures/default.png");
	
	material = { glm::vec3(1) };
}

void Scene::Update()
{
	std::shared_ptr<Shader> shader = assetManager.GetShader("Assets/Shaders/Shader");
	shader->BindShader();

	light.position = glm::vec3(1, -7, 0);
	light.direction = -glm::normalize(light.position);

	transformationManager.UpdateTransformationMatricies(&sceneGraph, &ECS);
}

void Scene::Render(Renderer* renderer)
{
	renderer->RenderSceneFromMainCamera(&ECS, &lightManager, assetManager.GetShader("Assets/Shaders/Shader").get(), assetManager.GetShader("Assets/Shaders/DepthShader").get(), &light);
}

void Scene::AddObject(unsigned int parent, std::string name, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale)
{
	unsigned int object = gameObjects[gameObjects.size() - 1] + 1;
	ECS.AddComponent<NameComponent>(object, name);
	ECS.AddComponent<Transform>(object, position, rotation, scale);
	ECS.AddComponent<MeshComponent>(object, assetManager.GetMesh("Assets/Models/monkey.obj").get(), assetManager.GetShader("Assets/Shaders/Shader").get(), &material, assetManager.GetTexture("Assets/Textures/rock.png").get());

	sceneGraph.InsertNode(object, sceneGraph.GetNode(parent, nullptr));
	gameObjects.push_back(object);
}


void Scene::DestroyScene()
{
	assetManager.GetShader("Assets/Shaders/DepthShader")->DestroyShader();
	assetManager.GetShader("Assets/Shaders/Shader")->DestroyShader();
}