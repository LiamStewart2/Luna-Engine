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
	glfwInit();

	window = Window::CreateWindow("Epic Game", SCREEN_WIDTH, SCREEN_HEIGHT);
	sceneManager.LoadNewScene("Assets/Scenes/example scene.json");

	imGuiLayer = ImGuiLayer(window, &sceneManager);
	
	return 0;
}

void Application::Terminate()
{
	glfwTerminate();
}

void Application::MainLoop()
{
	double lastTime = glfwGetTime();
	int frameCount = 0;

	while (!glfwWindowShouldClose(window->GetHandle()))
	{
		double currentTime = glfwGetTime();
		frameCount++;
		if (currentTime - lastTime >= 1.0)
		{
			std::string title = "FPS: " + std::to_string(frameCount);
			window->SetNewTitle(title.c_str());

			frameCount = 0;
			lastTime = currentTime;
		}

		HandleInput();

		Update();
		
		Render();

		window->Update();
	}
	sceneManager.UnloadCurrentScene();
	Window::CloseWindow(window);
}

void Application::HandleInput()
{
	if(window->GetKey(GLFW_KEY_F1) == GLFW_PRESS)
		Window::CloseWindow(window);
}

void Application::Update()
{
	imGuiLayer.Update();
	sceneManager.Update();
}

void Application::Render()
{

	glClearColor(0.7f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	sceneManager.Render(&renderer);

	imGuiLayer.Render();
}
