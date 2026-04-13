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
