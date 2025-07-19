#include "ImGuiLayer.h"

void ImGuiLayer::Init()
{
	m_MainScale = ImGui_ImplGlfw_GetContentScaleForMonitor(glfwGetPrimaryMonitor());

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
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

	{
		ImGui::Begin("Add Game Object");

		ImGui::InputFloat3("Position", m_PlacementPosition);
		ImGui::InputFloat3("Rotation", m_RotationPlacement);
		ImGui::InputFloat3("Scale", m_ScalePlacement);

		if (ImGui::Button("Add Object"))
			m_Scene->AddObject(glm::vec3(m_PlacementPosition[0], m_PlacementPosition[1], m_PlacementPosition[2]),
				glm::vec3(m_RotationPlacement[0], m_RotationPlacement[1], m_RotationPlacement[2]),
				glm::vec3(m_ScalePlacement[0], m_ScalePlacement[1], m_ScalePlacement[2]));

		ImGui::End();
	}

	{
		ImGui::Begin("Inspector");

		ImGui::InputInt("Game Object", &m_CurrentInspectorGameObject);

		ImGui::InputFloat("x", &m_Scene->GetECS()->GetObjectComponent<Transform>(m_CurrentInspectorGameObject)->position.x);
		ImGui::InputFloat("y", &m_Scene->GetECS()->GetObjectComponent<Transform>(m_CurrentInspectorGameObject)->position.y);
		ImGui::InputFloat("z", &m_Scene->GetECS()->GetObjectComponent<Transform>(m_CurrentInspectorGameObject)->position.z);

		ImGui::End();
	}
}

void ImGuiLayer::Render()
{
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
