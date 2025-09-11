#include "Scene.h"
#include "SceneManager.h"

Scene::Scene()
{

}

Scene::~Scene()
{
}

void Scene::Init(AssetManager* _assetManager, LightManager* _lightManager, std::string _sceneName)
{
	assetManager = _assetManager; lightManager = _lightManager; sceneName = _sceneName;
	LoadAssets();

	std::cout << "Loading Shader from scene" << std::endl;
	std::shared_ptr<Shader> shader = assetManager->GetShader("Assets/Shaders/Shader");

	shader->BindShader();
	shader->SetInt("diffuseTexture", 0);
	shader->SetInt("shadowMap", 1);
	
	unsigned int sceneObject = 0;
	ECS.AddComponent<NameComponent>(sceneObject, "Scene");
	ECS.AddComponent<Transform>(sceneObject, glm::vec3(0, 0, 0));
	sceneGraph = SceneGraph(sceneObject);
	gameObjects = std::vector<unsigned int>({0});
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



void Scene::Render(Renderer* renderer, FrameBuffer* framebuffer, ObjectTransformPairing<Camera> camera)
{
	//Find the Camera Object
	if (camera.object == nullptr)
	{
		std::unordered_map<unsigned int, CameraComponent*> cameras = ECS.GetAllComponentsOfType<CameraComponent>();
		unsigned int mainCameraID = 0;

		for (auto& [id, cameraComponent] : cameras)
		{
			auto cameraIt = cameras.find(cameraComponent->gameObject);
			if (cameraIt == cameras.end())
				return;
			else if(cameraComponent->m_MainCamera)
			{
				mainCameraID = id;
				camera.object = cameraComponent->m_Camera;
				camera.objectTransform = ECS.GetObjectComponent<Transform>(id);
			}
		}
		if (mainCameraID == 0)
		{
			std::cerr << "NO MAIN CAMERA" << std::endl;
			return;
		}
	}

	// Find the Light Object
	std::unordered_map<unsigned int, LightComponent*> lightComponents = ECS.GetAllComponentsOfType<LightComponent>();
	ObjectTransformPairing<LightComponent> light;
	for (auto& [id, LC] : lightComponents)
	{
		light.object = LC;
		light.objectTransform = ECS.GetObjectComponent<Transform>(id);
	}

	// Start Render Pass
	renderer->RenderPass(camera, light, ECS.GetAllComponentsOfType<Transform>(), ECS.GetAllComponentsOfType<MeshComponent>(),
	lightManager, assetManager->GetShader("Assets/Shaders/Shader").get(), assetManager->GetShader("Assets/Shaders/DepthShader").get(), framebuffer);
}

unsigned int Scene::AddObject(unsigned int parent)
{
	unsigned int object = gameObjects[gameObjects.size() - 1] + 1;

	sceneGraph.InsertNode(object, sceneGraph.GetNode(parent, nullptr));
	gameObjects.push_back(object);

	return object;
}

void Scene::DestroyGameObject(unsigned int gameObject)
{
	sceneGraph.RemoveNode(gameObject, &ECS, &gameObjects);
}


void Scene::DestroyScene()
{
	sceneGraph.RemoveNode(0, &ECS, &gameObjects);
}