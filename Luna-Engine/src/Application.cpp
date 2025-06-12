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

	// load scene
	shader = Shader("Assets/Shaders/Shader/shader.vs", "Assets/Shaders/Shader/shader.fs");

	AssetLoader::LoadMeshOBJ(monkeyMesh, "Assets/Models/monkey.obj");
	AssetLoader::LoadTexture(shrekTexture, "Assets/Textures/shrek.jpg");

	// Hide and set mouse position
	glfwSetCursorPos(window, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

	return 0;
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

#include <math.h>
void Application::Update()
{
	//camera.position.z = sin(glfwGetTime() * 0.5f) * -5;
}

void Application::Render()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	renderer.RenderMesh(&camera, &shader, &monkeyMesh);

	glfwSwapBuffers(window);
}
