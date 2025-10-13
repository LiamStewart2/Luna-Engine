#include "ImGuiLayer.h"
#include "DX11RendererContext.h"
#include "DX11Framebuffer.h"
#include <d3d11_4.h>

void ImGuiLayer::Init()
{
	m_MainScale = ImGui_ImplGlfw_GetContentScaleForMonitor(glfwGetPrimaryMonitor());

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
	//io.ConfigFlags &= -ImGuiConfigFlags_ViewportsEnable;

	ImGuiStyle& style = ImGui::GetStyle();
	style.ScaleAllSizes(m_MainScale);
	style.FontScaleDpi = m_MainScale;
	ImGui::StyleColorsDark();

	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}

	ImGui_ImplGlfw_InitForOther(m_Window->GetHandle(), true);
	Luna::DX11RendererContext* context = Luna::DX11RendererContext::GetContext();
	ImGui_ImplDX11_Init(context->GetDevice(), context->GetImmediateContext());
}

void ImGuiLayer::StartFrame()
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	ImGuizmo::BeginFrame();
}

// DOCKING IMPLEMENATION FROM THE CHERNO USING IMGUI DOCKING BRANCH
void ImGuiLayer::Update(ObjectTransformPairing<Camera>& camera, Luna::IFramebuffer* framebuffer, bool& runtime)
{
	StartFrame();

	Scene* scene = m_SceneManager->GetCurrentScene();

	static bool dockspaceOpen = true;
	static bool opt_fullscreen_persistant = true;
	bool opt_fullscreen = opt_fullscreen_persistant;
	static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

	// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
	// because it would be confusing to have two docking targets within each others.
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
	if (opt_fullscreen)
	{
		ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->Pos);
		ImGui::SetNextWindowSize(viewport->Size);
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
	}

	ImGuiIO& io = ImGui::GetIO();
	int fbWidth, fbHeight;
	glfwGetFramebufferSize(m_Window->GetHandle(), &fbWidth, &fbHeight);
	io.DisplaySize = ImVec2((float)fbWidth, (float)fbHeight);

	if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
		window_flags |= ImGuiWindowFlags_NoBackground;

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);
	ImGui::PopStyleVar();

	if (opt_fullscreen)
		ImGui::PopStyleVar(2);

	// DockSpace
	ImGuiStyle& style = ImGui::GetStyle();
	float minWinSizeX = style.WindowMinSize.x;
	style.WindowMinSize.x = 250.0f;
	style.WindowMinSize.y = 250.0f;
	if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
	{
		ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
	}

	style.WindowMinSize.x = minWinSizeX;

	if(ImGui::IsKeyDown(ImGuiKey_LeftCtrl) && ImGui::IsKeyDown(ImGuiKey_S) && !m_SaveShortcutUsed)
	{
		m_SaveShortcutUsed = true;
		m_SceneManager->SaveScene();
	}
	if(m_SaveShortcutUsed && (ImGui::IsKeyReleased(ImGuiKey_S) || ImGui::IsKeyReleased(ImGuiKey_LeftCtrl)))
		m_SaveShortcutUsed = false;

	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Save Scene", "CTRL+S"))
				m_SceneManager->SaveScene();
			if (ImGui::MenuItem("Save Scene As"))
			{
				std::string filepath = FileNavigation::SaveAsFileDialog({
					{L"Scene Files", L"*.json"},
					{L"All Files", L"*.*"}
					}, 1);
				if (!filepath.empty())
					m_SceneManager->SaveCurrentSceneAs(filepath);
			}
			if (ImGui::MenuItem("Load Scene"))
			{
				std::string filepath = FileNavigation::OpenFileDialog({
					{L"Scene Files", L"*.json"},
					{L"All Files", L"*.*"}
					}, 1);
				if (!filepath.empty())
					m_Actions.push_back({LOADSCENE, filepath});
			}
			ImGui::EndMenu();
		}


		if (ImGui::BeginMenu("View"))
		{
			/*
			if(ImGui::MenuItem("Scene"))
				m_ScenePanel.Open();
			if(ImGui::MenuItem("Game"))
				m_GamePanel.Open();
			if (ImGui::MenuItem("Inspector"))
				m_InspectorPanel.Open();
			if (ImGui::MenuItem("Content Browser"))
				m_ContentBrowserPanel.Open();
			if (ImGui::MenuItem("Hiearchy"))
				m_HierarchyPanel.Open();
			*/
			ImGui::EndMenu();
		}

		ImGui::EndMenuBar();
	}

	ImGui::ShowDemoWindow();

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
	ImGui::Begin("Scene");

	ImVec2 viewportPos = ImGui::GetWindowPos();
	ImVec2 viewportSize = ImGui::GetContentRegionAvail();

	// Center the framebuffer image in the window
	float aspectRatio = (float)16 / (float)9;

	ImVec2 imageSize{
		viewportSize.x,
		viewportSize.x / aspectRatio
	};
	if (viewportSize.y * aspectRatio < viewportSize.x)
	{
		imageSize.x = viewportSize.y * aspectRatio;
		imageSize.y = viewportSize.y;
	}
	

	ImVec2 imageOffset{
		(viewportSize.x - imageSize.x) * 0.5f,
		(viewportSize.y - imageSize.y) * 0.5f
	};

	
	ID3D11ShaderResourceView* srv = (ID3D11ShaderResourceView*)framebuffer->GetColorAttachment();

	ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPos().x + imageOffset.x, ImGui::GetCursorPos().y + imageOffset.y));
	ImGui::Image((ImTextureRef)srv, imageSize, ImVec2{ 0, 0 }, ImVec2{ 1, 1 });
	ImGui::End();
	ImGui::PopStyleVar();
	//m_InspectorPanel.Update(m_CurrentInspectorGameObject);
	//m_HierarchyPanel.Update(m_CurrentInspectorGameObject);
	//m_ContentBrowserPanel.Update(m_CurrentInspectorGameObject);

	//m_GamePanel.UpdateGame(m_CurrentInspectorGameObject, runtime);

	//m_ScenePanel.UpdateScene(m_CurrentInspectorGameObject, &m_Actions);
	//m_ScenePanel.UpdateGizmos(m_CurrentInspectorGameObject, camera);


	ImGui::End();

	//m_HierarchyPanel.EndFrame(m_CurrentInspectorGameObject);

	for (auto& [action, arguments] : m_Actions)
	{
		switch (action)
		{
		case ACTIONS::LOADSCENE:
			m_CurrentInspectorGameObject = 0;
			m_SceneManager->LoadNewScene(arguments.c_str());
			break;
		}
	}
	m_Actions.clear();
}

void ImGuiLayer::Render()
{
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		GLFWwindow* backup_current_context = glfwGetCurrentContext();
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		glfwMakeContextCurrent(backup_current_context);
	}
}
