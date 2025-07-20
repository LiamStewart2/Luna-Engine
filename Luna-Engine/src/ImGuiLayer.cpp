#include "ImGuiLayer.h"

void ImGuiLayer::Init()
{
	m_MainScale = ImGui_ImplGlfw_GetContentScaleForMonitor(glfwGetPrimaryMonitor());

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags &= ~ImGuiConfigFlags_NoMouseCursorChange;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

	ImGuiStyle& style = ImGui::GetStyle();
	style.ScaleAllSizes(m_MainScale);
	style.FontScaleDpi = m_MainScale;
	ImGui::StyleColorsDark();

	ImGui_ImplGlfw_InitForOpenGL(m_Window->GetHandle(), true);
	const char* glsl_version = "#version 460";
	ImGui_ImplOpenGL3_Init(glsl_version);
}

void ImGuiLayer::StartFrame()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void ImGuiLayer::Update()
{
	StartFrame();

	if (m_Window->GetMouseButton(GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
	{
		ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;
		m_Window->SetInputMode(GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
	}
	else
	{
		ImGui::GetIO().ConfigFlags &= ~ImGuiConfigFlags_NoMouseCursorChange;
		m_Window->SetInputMode(GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}

	{
		ImGui::Begin("Add Game Object");

		ImGui::InputInt("ParentObject", &m_ParentObject);

		ImGui::InputFloat3("Position", m_PlacementPosition);
		ImGui::InputFloat3("Rotation", m_RotationPlacement);
		ImGui::InputFloat3("Scale", m_ScalePlacement);

		if (ImGui::Button("Add Object"))
			m_Scene->AddObject((unsigned int)m_ParentObject,
				glm::vec3(m_PlacementPosition[0], m_PlacementPosition[1], m_PlacementPosition[2]),
				glm::vec3(m_RotationPlacement[0], m_RotationPlacement[1], m_RotationPlacement[2]),
				glm::vec3(m_ScalePlacement[0], m_ScalePlacement[1], m_ScalePlacement[2]));

		ImGui::End();
	}

	{
		ImGui::Begin("Inspector");

		ImGui::InputInt("Game Object", &m_CurrentInspectorGameObject);

		ImGui::Text("Position");
		ImGui::InputFloat("position x", &m_Scene->GetECS()->GetObjectComponent<Transform>(m_CurrentInspectorGameObject)->position.x);
		ImGui::InputFloat("position y", &m_Scene->GetECS()->GetObjectComponent<Transform>(m_CurrentInspectorGameObject)->position.y);
		ImGui::InputFloat("position z", &m_Scene->GetECS()->GetObjectComponent<Transform>(m_CurrentInspectorGameObject)->position.z);


		ImGui::Text("Rotation");
		ImGui::InputFloat("rotation x", &m_Scene->GetECS()->GetObjectComponent<Transform>(m_CurrentInspectorGameObject)->rotation.x);
		ImGui::InputFloat("rotation y", &m_Scene->GetECS()->GetObjectComponent<Transform>(m_CurrentInspectorGameObject)->rotation.y);
		ImGui::InputFloat("rotation z", &m_Scene->GetECS()->GetObjectComponent<Transform>(m_CurrentInspectorGameObject)->rotation.z);


		ImGui::Text("Scale");
		ImGui::InputFloat("scale x", &m_Scene->GetECS()->GetObjectComponent<Transform>(m_CurrentInspectorGameObject)->scale.x);
		ImGui::InputFloat("scale y", &m_Scene->GetECS()->GetObjectComponent<Transform>(m_CurrentInspectorGameObject)->scale.y);
		ImGui::InputFloat("scale z", &m_Scene->GetECS()->GetObjectComponent<Transform>(m_CurrentInspectorGameObject)->scale.z);

		ImGui::End();
	}

	{
		ImGui::Begin("Hieararcy");

		BuildHiearchyText(m_Scene->GetSceneGraph());

		ImGui::End();
	}
}

void ImGuiLayer::BuildHiearchyText(SceneGraphNode* node)
{
	ImGui::Text(std::to_string(node->GetGameObject()).c_str());
	ImGui::Indent(m_Indentation);
	for(SceneGraphNode* n : *node->getNodes())
		BuildHiearchyText(n);
	ImGui::Unindent(m_Indentation);
}

void ImGuiLayer::Render()
{
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
