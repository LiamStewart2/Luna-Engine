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

	window = LunaWindow::NewWindow("Epic Game", SCREEN_WIDTH, SCREEN_HEIGHT);

	sceneManager.InitFramebuffer();

	sceneManager.LoadNewScene("Assets/Scenes/second scene.json");

	imGuiLayer = ImGuiLayer(window, sceneManager.GetAssetManager(), &sceneManager);
	
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
	LunaWindow::CloseWindow(window);
}

void Application::HandleInput()
{
	if(window->GetKey(GLFW_KEY_F1) == GLFW_PRESS)
		LunaWindow::CloseWindow(window);
}

void Application::Update()
{
	sceneManager.Update();

	editorCamera.Update();

	Transform cameraTransform = Transform(0, editorCamera.m_Position, glm::quat(glm::radians(editorCamera.m_Rotation)));
	ObjectTransformPairing<Camera> cameraPair = {(Camera*)&editorCamera, &cameraTransform};
	imGuiLayer.Update(cameraPair);
}

void Application::Render()
{

	glClearColor(0.7f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	Transform cameraTransform = Transform(0, editorCamera.m_Position, glm::quat(glm::radians(editorCamera.m_Rotation)));
	ObjectTransformPairing<Camera> cameraPair = { (Camera*)&editorCamera, &cameraTransform };
	sceneManager.Render(&renderer, cameraPair);

	imGuiLayer.Render();
}
