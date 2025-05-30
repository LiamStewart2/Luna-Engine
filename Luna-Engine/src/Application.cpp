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
	window = glfwCreateWindow(640, 480, "Window", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	std::cout << glGetString(GL_VERSION) << std::endl;

	// load scene
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
	}
}

void Application::HandleInput()
{
	glfwPollEvents();
}

void Application::Update()
{
	
}

void Application::Render()
{
	glClear(GL_COLOR_BUFFER_BIT);



	glfwSwapBuffers(window);
}
