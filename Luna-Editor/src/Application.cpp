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

	m_Shader = Luna::IShader::Create("Assets/Shaders/SimpleShader/SimpleShaders.hlsl");

	std::vector<Luna::Vertex> vertexData = {
		{glm::vec3(-1.00f,  1.00f,  1), {1, 1}, {1, 1, 1}},
		{glm::vec3(1.00f,   1.00f,  1), {1, 1}, {1, 1, 1}},
		{glm::vec3(-1.00f, -1.00f,  1), {1, 1}, {1, 1, 1}},
		{glm::vec3(1.00f,  -1.00f,  1), {1, 1}, {1, 1, 1}},
		{glm::vec3(-1.00f,  1.00f, -1), {1, 1}, {1, 1, 1}},
		{glm::vec3(1.00f,   1.00f, -1), {1, 1}, {1, 1, 1}},
		{glm::vec3(-1.00f, -1.00f, -1), {1, 1}, {1, 1, 1}},
		{glm::vec3(1.00f,  -1.00f, -1), {1, 1}, {1, 1, 1}},
	};

	std::vector<unsigned int> indexData =
	{
		0, 1, 2,
		2, 1, 3,
		4, 5, 0,
		5, 1, 0,
		5, 4, 6,
		5, 6, 7,
		2, 7, 6,
		7, 2, 3,
		4, 0, 6,
		0, 2, 6,
		1, 5, 7,
		7, 3, 1
	};

	m_Mesh = Luna::IMesh::Create(vertexData, indexData);

	//sceneManager.LoadNewScene("Assets/Scenes/template scene.json");

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
	//sceneManager.UnloadCurrentScene();
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
	//sceneManager.GetCurrentScene()->GetScriptManager()->RecompileUpdatedScripts(sceneManager.GetCurrentScene()->GetECS());
	//sceneManager.Update(runtime);

	Transform cameraTransform = Transform(0, editorCamera.m_Position, glm::quat(glm::radians(editorCamera.m_Rotation)));
	ObjectTransformPairing<Camera> cameraPair = { (Camera*)&editorCamera, &cameraTransform };
	imGuiLayer.Update(cameraPair, m_Framebuffer.get(), runtime);

	//editorCamera.Update();
}

void Application::Render()
{

	//Transform cameraTransform = Transform(0, editorCamera.m_Position, glm::quat(glm::radians(editorCamera.m_Rotation)));
	//ObjectTransformPairing<Camera> cameraPair = { (Camera*)&editorCamera, &cameraTransform };
	//sceneManager.Render(&renderer, cameraPair, &m_SceneFramebuffer);

	//cameraPair = {nullptr, nullptr};
	//sceneManager.Render(&renderer, cameraPair, &m_GameFramebuffer);

	m_Framebuffer->Bind();
	float background[4] = {0.2f, 0.2f, 0.2f, 1.0f};
	m_Backbuffer->Clear(background); 
	m_Framebuffer->Clear(background);
	m_Shader->Bind();
	m_Mesh->BindMesh();

	Luna::ReworkedRenderer::BeginFrame(&sceneManager, m_Framebuffer.get());

	Luna::ReworkedRenderer::Render(&sceneManager, m_Framebuffer.get());
	
	Luna::ReworkedRenderer::EndFrame(&sceneManager, m_Framebuffer.get());

	m_Shader->Unbind();
	m_Framebuffer->Unbind();
	m_Backbuffer->Bind();

	imGuiLayer.Render();
}
