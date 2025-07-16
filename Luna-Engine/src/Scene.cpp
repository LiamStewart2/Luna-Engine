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
	ECS.AddComponent<Transform>(sceneObject, glm::vec3(0, 0, 0));

	unsigned int firstGameObject = 1;
	ECS.AddComponent<Transform>(firstGameObject, glm::vec3(2, -2, 0), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1));
	ECS.AddComponent<MeshComponent>(firstGameObject, &planeMesh, &shader, &material, &defaultTexture);

	unsigned int secondGameObject = 2;
	ECS.AddComponent<Transform>(secondGameObject, glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1));
	ECS.AddComponent<MeshComponent>(secondGameObject, &planeMesh, &shader, &material, &stoneTexture);

	unsigned int thirdGameObject = 3;
	ECS.AddComponent<Transform>(thirdGameObject, glm::vec3(0, 2, 0), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1));
	ECS.AddComponent<MeshComponent>(thirdGameObject, &monkeyMesh, &shader, &material, &stoneTexture);

	sceneGraph = SceneGraph(sceneObject);

	sceneGraph.InsertNode(firstGameObject);
	sceneGraph.InsertNode(secondGameObject);

	SceneGraphNode* node = sceneGraph.GetNode(secondGameObject, &sceneGraph);
	sceneGraph.InsertNode(thirdGameObject, node);
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

	ECS.GetObjectComponent<Transform>(1)->position.x = sin(glfwGetTime());
	ECS.GetObjectComponent<Transform>(2)->rotation.y = sin(glfwGetTime());

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


void Scene::DestroyScene()
{
	shader.DestroyShader();
}