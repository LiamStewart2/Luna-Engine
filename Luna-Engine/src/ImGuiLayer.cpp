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

		if(scene->GetECS()->HasComponent<NameComponent>(m_CurrentInspectorGameObject))
		{
			ImGui::SeparatorText("Name");

			ImGui::Text(scene->GetECS()->GetObjectComponent<NameComponent>(m_CurrentInspectorGameObject)->m_Name.c_str());
		}

		if (scene->GetECS()->HasComponent<Transform>(m_CurrentInspectorGameObject))
		{
			ImGui::SeparatorText("Transform");

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
		}

		if (scene->GetECS()->HasComponent<MeshComponent>(m_CurrentInspectorGameObject))
		{
			ImGui::SeparatorText("Mesh");

			ImGui::Text(scene->GetECS()->GetObjectComponent<MeshComponent>(m_CurrentInspectorGameObject)->mesh->path.c_str());
			ImGui::Text(scene->GetECS()->GetObjectComponent<MeshComponent>(m_CurrentInspectorGameObject)->shader->path.c_str());
			ImGui::Text(scene->GetECS()->GetObjectComponent<MeshComponent>(m_CurrentInspectorGameObject)->texture->path.c_str());
		}

		if (scene->GetECS()->HasComponent<CameraComponent>(m_CurrentInspectorGameObject))
		{
			ImGui::SeparatorText("Camera");

			ImGui::Checkbox("Main Camera", &scene->GetECS()->GetObjectComponent<CameraComponent>(m_CurrentInspectorGameObject)->m_MainCamera);
		}

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

	//ImGui::ShowDemoWindow();

	for (auto& [parent, name] : m_ObjectsToAdd)
	{
		m_SceneManager->AddObject(parent, name);
	}
	m_ObjectsToAdd.clear();

	// Process queued deletes
	for (unsigned int id : m_ObjectsToDelete)
	{
		m_SceneManager->GetCurrentScene()->DestroyGameObject(id);
		if (m_CurrentInspectorGameObject == id)
			m_CurrentInspectorGameObject = 0;
	}
	m_ObjectsToDelete.clear();
}

void ImGuiLayer::BuildHiearchyText(SceneGraphNode* node, std::unordered_map<unsigned int, NameComponent*>* names)
{
	unsigned int id = node->GetGameObject();
	std::string& name = names->at(id)->m_Name;
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

	if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0)) {
		m_RenamingGameObject = id;
		std::snprintf(m_RenameBuffer, sizeof(m_RenameBuffer), "%s", name.c_str());
	}

	if (ImGui::BeginPopupContextItem())
	{
		m_CurrentInspectorGameObject = id;
		if(ImGui::MenuItem("Add Object"))
			m_ObjectsToAdd.push_back({id, "New Object"});
		
		if (ImGui::BeginMenu("Add Component"))
		{
			ImGui::SeparatorText("Components");
			if(ImGui::MenuItem("Mesh"))
				m_SceneManager->GetCurrentScene()->AddComponent<MeshComponent>(m_CurrentInspectorGameObject, m_AssetManager->GetMesh("Assets/Models/planeobj.obj").get(), m_AssetManager->GetShader("Assets/Shaders/Shader").get(), new Material(glm::vec3(1, 1, 1)), m_AssetManager->GetTexture("Assets/Textures/default.png").get());

			if(ImGui::MenuItem("Camera"))
				m_SceneManager->GetCurrentScene()->AddComponent<CameraComponent>(m_CurrentInspectorGameObject, new PerspectiveCamera(), false);

			ImGui::EndMenu();
		}
			

		if (ImGui::MenuItem("Delete Object"))
			m_ObjectsToDelete.push_back({id});

		ImGui::EndPopup();
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
