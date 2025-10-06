#include "Application.h"

#include <iostream>

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
		1920, 1080,
		{Luna::FramebufferTextureFormat::RGBA8},
		Luna::DepthTextureFormat::DEPTH24STENCIL8,
		false
	};
	framebuffer = Luna::IFramebuffer::Create(framebufferSpec);

	//m_SceneFramebuffer = FrameBuffer(FramebufferSpecification(1920, 1080, std::vector<FramebufferTextureAttatchment>({ RGBA8, DEPTH })));
	//m_SceneFramebuffer.Update();
	//m_GameFramebuffer = FrameBuffer(FramebufferSpecification(1920, 1080, std::vector<FramebufferTextureAttatchment>({ RGBA8, DEPTH })));
	//m_GameFramebuffer.Update();

	//sceneManager.LoadNewScene("Assets/Scenes/template scene.json");

	//imGuiLayer = ImGuiLayer(window, sceneManager.GetAssetManager(), &sceneManager);
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
	//sceneManager.UnloadCurrentScene();
	LunaWindow::CloseWindow(window);
}

void Application::HandleInput()
{
	if(window->GetKey(GLFW_KEY_F1) == GLFW_PRESS)
		LunaWindow::CloseWindow(window);
}

void Application::Update()
{
	//sceneManager.GetCurrentScene()->GetScriptManager()->RecompileUpdatedScripts(sceneManager.GetCurrentScene()->GetECS());
	//sceneManager.Update(runtime);

	//Transform cameraTransform = Transform(0, editorCamera.m_Position, glm::quat(glm::radians(editorCamera.m_Rotation)));
	//ObjectTransformPairing<Camera> cameraPair = { (Camera*)&editorCamera, &cameraTransform };
	//imGuiLayer.Update(cameraPair, &m_SceneFramebuffer, &m_GameFramebuffer, runtime);

	//editorCamera.Update();
}

void Application::Render()
{

	//glClearColor(0.7f, 1.0f, 1.0f, 1.0f);
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Transform cameraTransform = Transform(0, editorCamera.m_Position, glm::quat(glm::radians(editorCamera.m_Rotation)));
	//ObjectTransformPairing<Camera> cameraPair = { (Camera*)&editorCamera, &cameraTransform };
	//sceneManager.Render(&renderer, cameraPair, &m_SceneFramebuffer);

	//cameraPair = {nullptr, nullptr};
	//sceneManager.Render(&renderer, cameraPair, &m_GameFramebuffer);

	//imGuiLayer.Render();

	framebuffer->Bind();
	Luna::ReworkedRenderer::BeginFrame();

	Luna::ReworkedRenderer::Render();
	
	Luna::ReworkedRenderer::EndFrame();
	framebuffer->Unbind();
}
