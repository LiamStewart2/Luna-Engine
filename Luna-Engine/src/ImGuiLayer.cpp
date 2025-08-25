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

	Scene* scene = m_SceneManager->GetCurrentScene();

	{
		ImGui::Begin("Inspector");

		ImGui::Text(scene->GetECS()->GetObjectComponent<NameComponent>(m_CurrentInspectorGameObject)->m_Name.c_str());

		ImGui::Text("Position");
		ImGui::InputFloat("position x", &scene->GetECS()->GetObjectComponent<Transform>(m_CurrentInspectorGameObject)->position.x);
		ImGui::InputFloat("position y", &scene->GetECS()->GetObjectComponent<Transform>(m_CurrentInspectorGameObject)->position.y);
		ImGui::InputFloat("position z", &scene->GetECS()->GetObjectComponent<Transform>(m_CurrentInspectorGameObject)->position.z);


		ImGui::Text("Rotation");
		ImGui::InputFloat("rotation x", &scene->GetECS()->GetObjectComponent<Transform>(m_CurrentInspectorGameObject)->rotation.x);
		ImGui::InputFloat("rotation y", &scene->GetECS()->GetObjectComponent<Transform>(m_CurrentInspectorGameObject)->rotation.y);
		ImGui::InputFloat("rotation z", &scene->GetECS()->GetObjectComponent<Transform>(m_CurrentInspectorGameObject)->rotation.z);


		ImGui::Text("Scale");
		ImGui::InputFloat("scale x", &scene->GetECS()->GetObjectComponent<Transform>(m_CurrentInspectorGameObject)->scale.x);
		ImGui::InputFloat("scale y", &scene->GetECS()->GetObjectComponent<Transform>(m_CurrentInspectorGameObject)->scale.y);
		ImGui::InputFloat("scale z", &scene->GetECS()->GetObjectComponent<Transform>(m_CurrentInspectorGameObject)->scale.z);

		ImGui::End();
	}

	{
		ImGui::Begin("Hieararcy");

		std::unordered_map<unsigned int, NameComponent*> names = scene->GetECS()->GetAllComponentsOfType<NameComponent>();
		BuildHiearchyText(scene->GetSceneGraph(), &names);

		ImGui::End();
	}

	{
		ImGui::Begin("File");

		ImGui::InputText("filepath: ", filepathForScene, 50);
		if(ImGui::Button("Save Scene"))
			m_SceneManager->SaveCurrentScene(filepathForScene);

		ImGui::End();
	}

	{
		ImGui::Begin("Add Object");

		ImGui::InputText("Object Name: ", m_ObjectName, 50);
		ImGui::InputInt("Parent Object ID:", &m_ParentObjectID);

		if(ImGui::Button("Add Object"))
			m_SceneManager->AddObject(m_ParentObjectID, std::string(m_ObjectName));

		ImGui::End();
	}

	{
		ImGui::Begin("Remove Object");
		

		ImGui::InputInt("Object ID:", &m_ObjectToRemoveID);
		if(ImGui::Button("Remove Object"))
		{
			m_SceneManager->GetCurrentScene()->DestroyGameObject(m_ObjectToRemoveID);
			m_CurrentInspectorGameObject = 0;
		}

		ImGui::End();
	}
}

void ImGuiLayer::BuildHiearchyText(SceneGraphNode* node, std::unordered_map<unsigned int, NameComponent*>* names)
{
	unsigned int id = node->GetGameObject();
	const std::string& name = names->at(id)->m_Name;
	bool selected = (id == m_CurrentInspectorGameObject);

	ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
	if (selected)
		flags |= ImGuiTreeNodeFlags_Selected;

	bool hasChildren = !node->getNodes()->empty();
	if (!hasChildren)
		flags |= ImGuiTreeNodeFlags_Leaf;

	bool nodeOpen = ImGui::TreeNodeEx((void*)(intptr_t)id, flags, "%s", name.c_str());

	if (ImGui::IsItemClicked()) {
		m_CurrentInspectorGameObject = id;
	}

	if (nodeOpen) {
		for (SceneGraphNode* child : *node->getNodes())
			BuildHiearchyText(child, names);
		ImGui::TreePop();
	}
}

void ImGuiLayer::Render()
{
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
