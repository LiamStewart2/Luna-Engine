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

	font = io.Fonts->AddFontFromFileTTF("Assets/Fonts/Roboto_Condensed-Bold.ttf", 15.0f);
	if(font == NULL)
		std::cerr << "CRASHED SILLY" << std::endl;

	ImGuiStyle& style = ImGui::GetStyle();
	ImGui::StyleColorsDark();
	style.ScaleAllSizes(m_MainScale);
	style.FontScaleDpi = m_MainScale;

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
	ImGui::PushFont(font);


	ImGuizmo::BeginFrame();
}

// DOCKING IMPLEMENATION FROM THE CHERNO USING IMGUI DOCKING BRANCH
void ImGuiLayer::Update(ObjectTransformPairing<Camera>& camera, std::shared_ptr<Luna::IFramebuffer> framebuffer, bool& runtime, std::shared_ptr<Luna::IFramebuffer> gameFramebuffer)
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

	// Geometry
	style.WindowPadding = ImVec2(10, 10);
	style.FramePadding = ImVec2(8, 5);
	style.ItemSpacing = ImVec2(8, 6);
	style.ItemInnerSpacing = ImVec2(6, 4);
	style.IndentSpacing = 20.0f;
	style.ScrollbarSize = 12.0f;

	style.WindowRounding = 6.0f;
	style.ChildRounding = 6.0f;
	style.FrameRounding = 4.0f;
	style.PopupRounding = 6.0f;
	style.ScrollbarRounding = 6.0f;
	style.GrabRounding = 4.0f;
	style.TabRounding = 4.0f;

	style.WindowBorderSize = 1.0f;
	style.ChildBorderSize = 1.0f;
	style.FrameBorderSize = 1.0f;
	style.PopupBorderSize = 1.0f;
	style.TabBorderSize = 0.0f;

	// Alignment
	style.WindowTitleAlign = ImVec2(0.0f, 0.5f);
	style.ButtonTextAlign = ImVec2(0.5f, 0.5f);

	// Colours
	ImVec4* c = style.Colors;

	// Text
	c[ImGuiCol_Text] = ImVec4(0.90f, 0.90f, 0.90f, 1.00f); // #E6E6E6
	c[ImGuiCol_TextDisabled] = ImVec4(0.55f, 0.55f, 0.55f, 1.00f); // #8C8C8C

	// Base
	c[ImGuiCol_WindowBg] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f); // #1A1A1A
	c[ImGuiCol_ChildBg] = ImVec4(0.08f, 0.08f, 0.08f, 1.00f); // #141414
	c[ImGuiCol_PopupBg] = ImVec4(0.12f, 0.12f, 0.12f, 0.98f); // #1F1F1F

	// Borders / separators
	c[ImGuiCol_Border] = ImVec4(0.18f, 0.18f, 0.18f, 1.00f); // #2E2E2E
	c[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	c[ImGuiCol_Separator] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f); // #333333
	c[ImGuiCol_SeparatorHovered] = ImVec4(0.32f, 0.32f, 0.32f, 1.00f); // #525252
	c[ImGuiCol_SeparatorActive] = ImVec4(0.38f, 0.38f, 0.38f, 1.00f); // #616161

	// Frames (inputs, sliders, etc.)
	c[ImGuiCol_FrameBg] = ImVec4(0.16f, 0.16f, 0.16f, 1.00f); // #292929
	c[ImGuiCol_FrameBgHovered] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f); // #333333
	c[ImGuiCol_FrameBgActive] = ImVec4(0.24f, 0.24f, 0.24f, 1.00f); // #3D3D3D

	// Title / menu
	c[ImGuiCol_TitleBg] = ImVec4(0.07f, 0.07f, 0.07f, 1.00f); // #121212
	c[ImGuiCol_TitleBgActive] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f); // #1A1A1A
	c[ImGuiCol_TitleBgCollapsed] = ImVec4(0.07f, 0.07f, 0.07f, 1.00f);
	c[ImGuiCol_MenuBarBg] = ImVec4(0.09f, 0.09f, 0.09f, 1.00f); // #171717

	// Scrollbar
	c[ImGuiCol_ScrollbarBg] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
	c[ImGuiCol_ScrollbarGrab] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f); // #404040
	c[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.32f, 0.32f, 0.32f, 1.00f);
	c[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.38f, 0.38f, 0.38f, 1.00f);

	// Check / radio / slider
	c[ImGuiCol_CheckMark] = ImVec4(0.85f, 0.85f, 0.85f, 1.00f);
	c[ImGuiCol_SliderGrab] = ImVec4(0.55f, 0.55f, 0.55f, 1.00f);
	c[ImGuiCol_SliderGrabActive] = ImVec4(0.75f, 0.75f, 0.75f, 1.00f);

	// Buttons
	c[ImGuiCol_Button] = ImVec4(0.18f, 0.18f, 0.18f, 1.00f); // #2E2E2E
	c[ImGuiCol_ButtonHovered] = ImVec4(0.24f, 0.24f, 0.24f, 1.00f); // #3D3D3D
	c[ImGuiCol_ButtonActive] = ImVec4(0.30f, 0.30f, 0.30f, 1.00f); // #4D4D4D

	// Headers (tree, selectable)
	c[ImGuiCol_Header] = ImVec4(0.16f, 0.16f, 0.16f, 1.00f);
	c[ImGuiCol_HeaderHovered] = ImVec4(0.22f, 0.22f, 0.22f, 1.00f);
	c[ImGuiCol_HeaderActive] = ImVec4(0.28f, 0.28f, 0.28f, 1.00f);

	// Tabs (docking)
	c[ImGuiCol_Tab] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
	c[ImGuiCol_TabHovered] = ImVec4(0.22f, 0.22f, 0.22f, 1.00f);
	c[ImGuiCol_TabActive] = ImVec4(0.18f, 0.18f, 0.18f, 1.00f);
	c[ImGuiCol_TabUnfocused] = ImVec4(0.12f, 0.12f, 0.12f, 1.00f);
	c[ImGuiCol_TabUnfocusedActive] = ImVec4(0.16f, 0.16f, 0.16f, 1.00f);

	// Docking
	c[ImGuiCol_DockingPreview] = ImVec4(0.70f, 0.70f, 0.70f, 0.20f);
	c[ImGuiCol_DockingEmptyBg] = ImVec4(0.06f, 0.06f, 0.06f, 1.00f);

	// Tables
	c[ImGuiCol_TableHeaderBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
	c[ImGuiCol_TableBorderStrong] = ImVec4(0.22f, 0.22f, 0.22f, 1.00f);
	c[ImGuiCol_TableBorderLight] = ImVec4(0.18f, 0.18f, 0.18f, 1.00f);
	c[ImGuiCol_TableRowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	c[ImGuiCol_TableRowBgAlt] = ImVec4(1.00f, 1.00f, 1.00f, 0.03f);

	// Selection / highlights
	c[ImGuiCol_TextSelectedBg] = ImVec4(0.70f, 0.70f, 0.70f, 0.25f);
	c[ImGuiCol_DragDropTarget] = ImVec4(0.80f, 0.80f, 0.80f, 0.90f);
	c[ImGuiCol_NavHighlight] = ImVec4(0.80f, 0.80f, 0.80f, 0.60f);
	c[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);

	// Modal
	c[ImGuiCol_ModalWindowDimBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.50f);


	if(ImGui::IsKeyDown(ImGuiKey_LeftCtrl) && ImGui::IsKeyDown(ImGuiKey_S) && !m_SaveShortcutUsed)
	{
		m_SaveShortcutUsed = true;
		m_SceneManager->SaveScene();
		m_MaterialEditorPanel.SaveCurrentMaterial();
	}
	if(m_SaveShortcutUsed && (ImGui::IsKeyReleased(ImGuiKey_S) || ImGui::IsKeyReleased(ImGuiKey_LeftCtrl)))
		m_SaveShortcutUsed = false;

	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("New Project"))
			{
				m_NewProjectPanel.Open();
			}

			if (ImGui::MenuItem("Open Project"))
			{
				std::string fpath = m_ProjectManager->NavigateFolders();
				if (!fpath.empty())
				{
					m_ProjectManager->OpenProject(fpath);
					m_SceneManager->SetAssetWorkingPath(m_ProjectManager->GetOpenProject().m_WorkingDirectory);
					m_ContentBrowserPanel.ResetDirectory(m_ProjectManager->GetOpenProject().m_WorkingDirectory);
					m_SceneManager->LoadNewScene(m_ProjectManager->GetOpenProject().m_DefaultScenePath.c_str());
				}
			}

			if (ImGui::MenuItem("Save Scene", "CTRL+S"))
				m_SceneManager->SaveScene();
			if (ImGui::MenuItem("Save Scene As"))
			{
				std::string filepath = FileNavigation::SaveAsFileDialog({
					{L"Scene Files", L"*.json"},
					{L"All Files", L"*.*"}
					}, 1);
				if (!filepath.empty())
					m_SceneManager->SaveScene(nullptr, filepath);
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
			if(ImGui::MenuItem("Material Editor"))
				m_MaterialEditorPanel.Open();
			if (ImGui::MenuItem("Shader Graph"))
				m_ShaderGraphPanel.Open();

			ImGui::EndMenu();
		}

		ImGui::EndMenuBar();
	}

	ImGui::ShowDemoWindow();

	m_InspectorPanel.Update(m_CurrentInspectorGameObject);
	m_HierarchyPanel.Update(m_CurrentInspectorGameObject);
	m_ContentBrowserPanel.Update(m_CurrentInspectorGameObject);
	m_MaterialEditorPanel.Update(m_CurrentInspectorGameObject);

	m_GamePanel.UpdateGame(m_CurrentInspectorGameObject, runtime, gameFramebuffer);

	m_ScenePanel.UpdateScene(m_CurrentInspectorGameObject, framebuffer, &m_Actions);
	m_ScenePanel.UpdateGizmos(m_CurrentInspectorGameObject, camera);

	m_ShaderGraphPanel.Update(m_CurrentInspectorGameObject);

	m_NewProjectPanel.Update(&m_ContentBrowserPanel, &m_Actions);


	ImGui::PopFont();
	ImGui::End();
	

	m_HierarchyPanel.EndFrame(m_CurrentInspectorGameObject);

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
