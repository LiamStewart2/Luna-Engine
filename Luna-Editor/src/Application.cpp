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

	m_Shader = Luna::IShader::Create("Assets/Shaders/SimpleShader/SimpleShaders.hlsl");

	std::vector<Luna::Vertex> vertexData = {
		// Front Face 0-3
		{glm::vec3(-1.00f,  1.00f,  1), {0, 0}, {0.00f,   0.00f,  1.00f}}, // 0
		{glm::vec3(1.00f,   1.00f,  1), {1, 0}, {0.00f,   0.00f,  1.00f}}, // 1
		{glm::vec3(-1.00f, -1.00f,  1), {0, 1}, {0.00f,   0.00f,  1.00f}}, // 2
		{glm::vec3(1.00f,  -1.00f,  1), {1, 1}, {0.00f,   0.00f,  1.00f}}, // 3

		//Right Face 4-7
		{glm::vec3(1.00f,   1.00f,  1), {0, 0}, {1.00f,   0.00f,  0.00f}}, // 4
		{glm::vec3(1.00f,  -1.00f,  1), {1, 0}, {1.00f,   0.00f,  0.00f}}, // 5
		{glm::vec3(1.00f,   1.00f, -1), {0, 1}, {1.00f,   0.00f,  0.00f}}, // 6
		{glm::vec3(1.00f,  -1.00f, -1), {1, 1}, {1.00f,   0.00f,  0.00f}}, // 7

		//Back Face 8-11
		{glm::vec3(-1.00f,  1.00f, -1), {0, 0}, {0.00f, 0.00f, -1.00f}}, //8
		{glm::vec3(1.00f,   1.00f, -1), {1, 0}, {0.00f, 0.00f, -1.00f}}, // 9
		{glm::vec3(-1.00f, -1.00f, -1), {0, 1}, {0.00f, 0.00f, -1.00f}}, // 10
		{glm::vec3(1.00f,  -1.00f, -1), {1, 1}, {0.00f, 0.00f, -1.00f}}, // 11

		// Left Face 12-15
		{glm::vec3(-1.00f,   1.00f,  1), {0, 0}, {-1.00f,   0.00f,  0.00f}}, // 12
		{glm::vec3(-1.00f,  -1.00f,  1), {1, 0}, {-1.00f,   0.00f,  0.00f}}, // 13
		{glm::vec3(-1.00f,   1.00f, -1), {0, 1}, {-1.00f,   0.00f,  0.00f}}, // 14
		{glm::vec3(-1.00f,  -1.00f, -1), {1, 1}, {-1.00f,   0.00f,  0.00f}}, // 15

		//Top Face 16-19
		{glm::vec3(-1.00f,  1.00f,  1), {0, 0}, {0.00f,   1.00f, 0.00f}}, // 16
		{glm::vec3(1.00f,   1.00f,  1), {1, 0}, {0.00f,   1.00f, 0.00f}}, // 17
		{glm::vec3(-1.00f,  1.00f, -1), {0, 1}, {0.00f,   1.00f, 0.00f}}, // 18
		{glm::vec3(1.00f,   1.00f, -1), {1, 1}, {0.00f,   1.00f, 0.00f}}, // 19

		//Bottom Face
		{glm::vec3(-1.00f,  -1.00f,  1), {0, 0}, {0.00f,   -1.00f, 0.00f}}, // 20
		{glm::vec3(1.00f,   -1.00f,  1), {1, 0}, {0.00f,   -1.00f, 0.00f}}, // 21
		{glm::vec3(-1.00f,  -1.00f, -1), {0, 1}, {0.00f,   -1.00f, 0.00f}}, // 22
		{glm::vec3(1.00f,   -1.00f, -1), {1, 1}, {0.00f,   -1.00f, 0.00f}}, // 23
	};

	std::vector<unsigned int> indexData =
	{
		0, 1, 2,
		1, 3, 2,
		4, 6, 5,
		6, 7, 5,
		9, 8, 11,
		8, 10, 11,
		14, 12, 15,
		12, 13, 15,
		18, 19, 16,
		19, 17, 16,
		20, 21, 22,
		21, 23, 22,
	};

	m_Mesh = Luna::IMesh::Create(vertexData, indexData);


	Luna::TexturePacket texturePacket;
	texturePacket.path = "Assets/Textures/Grass.jpg";
	
	texturePacket.buffer = stbi_load(texturePacket.path.c_str(), &texturePacket.width, &texturePacket.height, &texturePacket.channels, STBI_rgb_alpha);
	if (!texturePacket.buffer)
	{
		std::cerr << "Failed to load texture - " << texturePacket.path << std::endl;
	}
	m_Texture = Luna::ITexture::Create(texturePacket);
	stbi_image_free(texturePacket.buffer);

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

	editorCamera.Update();
}

void Application::Render()
{

	Transform cameraTransform = Transform(0, editorCamera.m_Position, glm::quat(glm::radians(editorCamera.m_Rotation)));
	ObjectTransformPairing<Camera> cameraPair = { (Camera*)&editorCamera, &cameraTransform };
	//sceneManager.Render(&renderer, cameraPair, &m_SceneFramebuffer);

	//cameraPair = {nullptr, nullptr};
	//sceneManager.Render(&renderer, cameraPair, &m_GameFramebuffer);

	m_Framebuffer->Bind();
	float background[4] = {0.2f, 0.2f, 0.2f, 1.0f};
	m_Backbuffer->Clear(background); 
	m_Framebuffer->Clear(background);
	m_Shader->Bind();
	m_Mesh->BindMesh();
	m_Texture->BindTexture();

	Luna::ReworkedRenderer::BeginFrame(&sceneManager, m_Framebuffer.get(), &cameraPair);

	Luna::ReworkedRenderer::Render(&sceneManager, m_Framebuffer.get());
	
	Luna::ReworkedRenderer::EndFrame(&sceneManager, m_Framebuffer.get());

	m_Shader->Unbind();
	m_Framebuffer->Unbind();
	m_Backbuffer->Bind();

	imGuiLayer.Render();
}
