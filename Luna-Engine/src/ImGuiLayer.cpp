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

		ImGui::InputFloat3("position", m_PlacementPosition);
		if (ImGui::Button("Add Object"))
			m_Scene->AddObject(m_PlacementPosition[0], m_PlacementPosition[1], m_PlacementPosition[2]);

		ImGui::Text("epicccc");

		ImGui::End();
	}
}

void ImGuiLayer::Render()
{
	std::cout << "fortnite";
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
