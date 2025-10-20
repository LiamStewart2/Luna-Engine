#include "Application.h"

#include <iostream>

//#define STB_IMAGE_IMPLEMENTATION
#include "STB/stb_image.h"

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
	
	m_RendererContext = Luna::RendererContext::Create((void*)window->GetHandle());

	Luna::ReworkedRenderer::Init(m_RendererContext);

	Luna::FramebufferSpecification framebufferSpec = {
		SCREEN_WIDTH, SCREEN_HEIGHT,
		{Luna::FramebufferTextureFormat::RGBA8},
		Luna::DepthTextureFormat::DEPTH24STENCIL8,
		false
	};
	Luna::FramebufferSpecification backbufferspec = {
		SCREEN_WIDTH, SCREEN_HEIGHT,
		{Luna::FramebufferTextureFormat::RGBA8},
		Luna::DepthTextureFormat::DEPTH24STENCIL8,
		true
	};
	m_Framebuffer = Luna::IFramebuffer::Create(framebufferSpec);
	m_Backbuffer = Luna::IFramebuffer::Create(backbufferspec);


	sceneManager.LoadNewScene("Assets/Scenes/template scene.json");

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

	while (!window->ShouldClose())
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
		m_RendererContext->SwapBuffers();
	}
	sceneManager.UnloadCurrentScene();
	LunaWindow::CloseWindow(window);
}

void Application::HandleInput()
{
	if(window->GetKey(GLFW_KEY_F1) == GLFW_PRESS)
		LunaWindow::CloseWindow(window);
}

void Application::ResizeCallback(GLFWwindow* handle, int width, int height)
{
	m_Backbuffer->Resize(width, height);
}

void Application::Update()
{
	sceneManager.GetCurrentScene()->GetScriptManager()->RecompileUpdatedScripts(sceneManager.GetCurrentScene()->GetECS());
	sceneManager.Update(runtime);

	Transform cameraTransform = Transform(0, editorCamera.m_Position, glm::quat(glm::radians(editorCamera.m_Rotation)));
	ObjectTransformPairing<Camera> cameraPair = { (Camera*)&editorCamera, &cameraTransform };
	imGuiLayer.Update(cameraPair, m_Framebuffer, runtime);

	editorCamera.Update();
}

void Application::Render()
{

	Transform cameraTransform = Transform(0, editorCamera.m_Position, glm::quat(glm::radians(editorCamera.m_Rotation)));
	ObjectTransformPairing<Camera> cameraPair = { (Camera*)&editorCamera, &cameraTransform };

	m_Framebuffer->Bind();

	float background[4] = {0.2f, 0.2f, 0.2f, 1.0f};
	m_Backbuffer->Clear(background); 
	m_Framebuffer->Clear(background);

	Luna::ReworkedRenderer::BeginFrame(&sceneManager, m_Framebuffer.get(), &cameraPair);

	Luna::ReworkedRenderer::Render(&sceneManager, m_Framebuffer.get());
	
	Luna::ReworkedRenderer::EndFrame(&sceneManager, m_Framebuffer.get());

	m_Framebuffer->Unbind();
	m_Backbuffer->Bind();

	imGuiLayer.Render();
}
