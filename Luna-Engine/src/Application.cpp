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
	window = glfwCreateWindow(1280, 720, "Window", NULL, NULL);
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
		HandleInput();
		Update();
		Render();

		glfwPollEvents();
	}
}

void Application::HandleInput()
{

}

#include <math.h>
void Application::Update()
{
	camera.position.z = sin(glfwGetTime() * 0.5f) * -5;
}

void Application::Render()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	renderer.RenderMesh(&camera, &shader, &monkeyMesh);

	glfwSwapBuffers(window);
}
