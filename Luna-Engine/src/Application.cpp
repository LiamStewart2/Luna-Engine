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

	main_scale = ImGui_ImplGlfw_GetContentScaleForMonitor(glfwGetPrimaryMonitor());
	window = Window::CreateWindow("Epic Game", SCREEN_WIDTH, SCREEN_HEIGHT);
	window->SetCursorPosition(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
	window->SetInputMode(GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

	ImGuiStyle& style = ImGui::GetStyle();
	style.ScaleAllSizes(main_scale);
	style.FontScaleDpi = main_scale;
	ImGui::StyleColorsDark();

	scene.Init(window);

	ImGui_ImplGlfw_InitForOpenGL(window->GetHandle(), true);
	const char* glsl_version = "#version 460";
	ImGui_ImplOpenGL3_Init(glsl_version);

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

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		HandleInput();

		Update();
		
		Render();

		window->Update();
	}
	scene.DestroyScene();
	Window::CloseWindow(window);
}

void Application::HandleInput()
{
	if(window->GetKey(GLFW_KEY_F1) == GLFW_PRESS)
		Window::CloseWindow(window);
}

void Application::Update()
{
	{
		ImGui::Begin("Add Game Object");

		ImGui::InputFloat3("position", placementPosition);
		if(ImGui::Button("Add Object"))
			scene.AddObject(placementPosition[0], placementPosition[1], placementPosition[2]);

		ImGui::Text("epicccc");

		ImGui::End();
	}
	scene.Update();
}

void Application::Render()
{
	ImGui::Render();

	glClearColor(0.7f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	scene.Render(&renderer);
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

}
