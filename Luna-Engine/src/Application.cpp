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
	_window = Window::CreateWindow("Epic Game", SCREEN_WIDTH, SCREEN_HEIGHT);

	scene.Init(window);

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
	double lastTime = glfwGetTime();
	int frameCount = 0;

	while (_window->IsRunning())
	{
		double currentTime = glfwGetTime();
		frameCount++;
		if (currentTime - lastTime >= 1.0)
		{
			std::string title = "FPS: " + std::to_string(frameCount);
			glfwSetWindowTitle(window, title.c_str());

			frameCount = 0;
			lastTime = currentTime;
		}

		HandleInput();

		Update();
		
		Render();

		_window->Update();
	}
	scene.DestroyScene();
}

void Application::HandleInput()
{
	if(_window->GetKey(GLFW_KEY_F1) == GLFW_PRESS)
		Window::CloseWindow(_window);
}

void Application::Update()
{
	scene.Update();
}

void Application::Render()
{
	glClearColor(0.7f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	scene.Render(&renderer);
}
