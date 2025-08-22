#include "Scene.h"
#include "SceneManager.h"

Scene::Scene()
{

}

Scene::~Scene()
{
}

void Scene::Init(AssetManager* _assetManager, std::string _sceneName)
{
	assetManager = _assetManager; sceneName = _sceneName;
	LoadAssets();

	std::shared_ptr<Shader> shader = assetManager->GetShader("Assets/Shaders/Shader");

	shader->BindShader();
	shader->SetInt("diffuseTexture", 0);
	shader->SetInt("shadowMap", 1);
	
	unsigned int sceneObject = 0;
	ECS.AddComponent<NameComponent>(sceneObject, "Scene");
	ECS.AddComponent<Transform>(sceneObject, glm::vec3(0, 0, 0));
	sceneGraph = SceneGraph(sceneObject);
	gameObjects = std::vector<unsigned int>({0});

	unsigned int plane = 1;
	ECS.AddComponent<NameComponent>(plane, "Plane");
	ECS.AddComponent<Transform>(plane, glm::vec3(0, -2, 0), glm::vec3(0, 0, 0), glm::vec3(5, 1, 5));
	ECS.AddComponent<MeshComponent>(plane, assetManager->GetMesh("Assets/Models/planeobj.obj").get(), shader.get(), &material, assetManager->GetTexture("Assets/Textures/default.png").get());
	sceneGraph.InsertNode(plane);
	gameObjects.push_back(plane);

	unsigned int camera = 2;
	ECS.AddComponent<NameComponent>(camera, "Camera");
	ECS.AddComponent<Transform>(camera, glm::vec3(0, 2, 5), glm::vec3(20, 180, 0), glm::vec3(1, 1, 1));
	ECS.AddComponent<CameraComponent>(camera, perspectiveCamera, true);
	lightManager = LightManager(perspectiveCamera);
	sceneGraph.InsertNode(camera);
	gameObjects.push_back(camera);

	unsigned int light = 3;
	ECS.AddComponent<NameComponent>(light, "Light");
	ECS.AddComponent<Transform>(light, glm::vec3(0, 5, 0), glm::vec3(-90, 30, 0), glm::vec3(1, 1, 1));
	ECS.AddComponent<LightComponent>(light, glm::vec3(1, 1, 1));
	ECS.GetObjectComponent<LightComponent>(light)->m_Light.BuildLight(&lightManager);
	sceneGraph.InsertNode(light);
	gameObjects.push_back(light);
}

void Scene::LoadAssets()
{
	material = { glm::vec3(1) };
}

void Scene::Update()
{
	std::shared_ptr<Shader> shader = assetManager->GetShader("Assets/Shaders/Shader");
	shader->BindShader();

	transformationManager.UpdateTransformationMatricies(&sceneGraph, &ECS);
}



void Scene::Render(Renderer* renderer)
{
	renderer->RenderSceneFromMainCamera(&ECS, &lightManager, assetManager->GetShader("Assets/Shaders/Shader").get(), assetManager->GetShader("Assets/Shaders/DepthShader").get());
}

unsigned int Scene::AddObject(unsigned int parent)
{
	unsigned int object = gameObjects[gameObjects.size() - 1] + 1;

	sceneGraph.InsertNode(object, sceneGraph.GetNode(parent, nullptr));
	gameObjects.push_back(object);

	return object;
}


void Scene::DestroyScene()
{
	sceneGraph.RemoveNode(0, nullptr);

	assetManager->GetShader("Assets/Shaders/DepthShader")->DestroyShader();
	assetManager->GetShader("Assets/Shaders/Shader")->DestroyShader();
}