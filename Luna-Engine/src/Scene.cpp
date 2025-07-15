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
	ECS.AddComponent<Transform>(sceneObject, Transform());

	unsigned int firstGameObject = 1;
	ECS.AddComponent<Transform>(firstGameObject, glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(10, 1, 10));
	ECS.AddComponent<MeshComponent>(firstGameObject, &planeMesh, &shader, &material, &defaultTexture);

	unsigned int secondGameObject = 2;
	ECS.AddComponent<Transform>(secondGameObject, glm::vec3(0, 2, 0));
	ECS.AddComponent<MeshComponent>(secondGameObject, &monkeyMesh, &shader, &material, &stoneTexture);

	sceneGraph = SceneGraph(sceneObject);

	sceneGraph.InsertNode(firstGameObject);

	SceneGraphNode* node = sceneGraph.GetNode(firstGameObject, &sceneGraph);

	sceneGraph.InsertNode(secondGameObject, node);

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
	transformationManager.UpdateTransformationMatricies(&sceneGraph, &ECS);

	shader.BindShader();
	if(window->GetMouseButton(GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
		shader.SetInt("debugMode", 1);
	else
		shader.SetInt("debugMode", 0);

	light.position = glm::vec3(5, -5, 0);
	light.direction = -glm::normalize(light.position);
	camera.HandleInput(window);
}

void Scene::Render(Renderer* renderer)
{
	std::vector<Transform*> transforms = ECS.GetAllComponentsOfType<Transform>();
	std::vector<MeshComponent*> meshComponents = ECS.GetAllComponentsOfType<MeshComponent>();

	light.FrameSetup(&lightManager, &depthmapShader, &shader);
	for (size_t i = 0; i < meshComponents.size(); i++)
		light.RenderObjectToDepthmap(meshComponents[i]->mesh, transforms[i], &depthmapShader);
	light.FrameReset();
	
	renderer->SetShaderFrame(&camera, &depthmapShader, &shader, &light);
	for(size_t i = 0; i < meshComponents.size(); i++)
		renderer->RenderObject(transforms[i], meshComponents[i]->mesh, meshComponents[i]->texture, meshComponents[i]->material, meshComponents[i]->shader);
}


void Scene::DestroyScene()
{
	shader.DestroyShader();
}