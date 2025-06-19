#include "Application.h"

Application::Application()
{
	Init();
	MainLoop();
}

Application::~Application()
{
	Terminate();
}

int Application::Init()
{
	if(!glfwInit())
		return -1;
	window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Window", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	if (glewInit() != GLEW_OK)
	{
		std::cerr << "Failed to initialize GLEW" << std::endl;
		return -1;
	}

	std::cout << glGetString(GL_VERSION) << std::endl;

	LoadAssets();

	// Hide and set mouse position
	glfwSetCursorPos(window, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

	glActiveTexture(GL_TEXTURE0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);

	return 0;
}

void Application::LoadAssets()
{
	//Load all assets for the scene
	shader = Shader("Assets/Shaders/Shader/shader.vs", "Assets/Shaders/Shader/shader.fs");

	AssetLoader::LoadMeshOBJ(monkeyMesh, "Assets/Models/monkey.obj");
	AssetLoader::LoadMeshOBJ(smoothMonkeyMesh, "Assets/Models/smoothMonkey.obj");

	AssetLoader::LoadTexture(shrekTexture, "Assets/Textures/rock.png");

	light = {glm::vec3(0), glm::vec3(1)};
	material = {glm::vec3(1)};
}

void Application::Terminate()
{
	glfwTerminate();
}

void Application::MainLoop()
{
	while (!glfwWindowShouldClose(window))
	{
		Time::SetStartTime(glfwGetTime());

		HandleInput();

		Update();
		
		Render();

		glfwPollEvents();

		Time::SetEndTime(glfwGetTime());
	}
	shader.DestroyShader();
}

void Application::HandleInput()
{
	camera.HandleInput(window);

	if (glfwGetKey(window, GLFW_KEY_F1) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

void Application::Update()
{
	light.position = glm::vec3(cos(glfwGetTime()) * 5, 0, sin(glfwGetTime()) * 5);
}

void Application::Render()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	shrekTexture.BindTexture(shader);

	renderer.SetupFrame(&camera, &shader, &light);
	renderer.RenderMesh(&camera, &shader, &smoothMonkeyMesh, &material);

	glfwSwapBuffers(window);
}
