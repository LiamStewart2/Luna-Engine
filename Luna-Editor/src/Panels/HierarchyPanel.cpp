#include "HierarchyPanel.h"

void HierarchyPanel::Update(unsigned int& inspectorID)
{
	if(m_Show == false)
		return;

	ImGui::Begin("Hieararcy", &m_Show);

	Scene* scene = m_SceneManager->GetCurrentScene();

	ImGui::SeparatorText(scene->GetSceneName().c_str());

	std::unordered_map<unsigned int, NameComponent>& names = scene->GetECS()->GetAllComponentsOfType<NameComponent>();

	bool hasRightClicked = false;

	SceneGraphNode* sceneNode = scene->GetSceneGraph();
	for (int i = 0; i < sceneNode->getNodes()->size(); i++)
		BuildHiearchyText(inspectorID, scene->GetSceneGraph()->GetNode(sceneNode->getNodes()->at(i)->GetGameObject(), nullptr), names, hasRightClicked);

	if (!hasRightClicked && ImGui::BeginPopupContextWindow(0, 1))
	{
		if (ImGui::MenuItem("Add Object"))
			m_ObjectsToAdd.push_back({ 0, "New Object" });

		ImGui::EndPopup();
	}

	ImGui::End();
}

void HierarchyPanel::BuildHiearchyText(unsigned int& inspectorID, SceneGraphNode* node, std::unordered_map<unsigned int, NameComponent>& names, bool& HasBeenRightClicked)
{
	unsigned int id = node->GetGameObject();
	std::string& name = names[id].m_Name;
	bool selected = (id == inspectorID);

	ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
	if (selected)
		flags |= ImGuiTreeNodeFlags_Selected;

	bool hasChildren = !node->getNodes()->empty();
	if (!hasChildren)
		flags |= ImGuiTreeNodeFlags_Leaf;

	bool nodeOpen = ImGui::TreeNodeEx((void*)(intptr_t)id, flags, "%s", name.c_str());

	if (ImGui::IsItemClicked()) {
		inspectorID = id;
	}

	if (ImGui::BeginPopupContextItem())
	{
		inspectorID = id;
		HasBeenRightClicked = true;

		if (ImGui::MenuItem("Add Object"))
		{
			m_ObjectsToAdd.push_back({ id, "New Object" });
		}

		if (ImGui::BeginMenu("Add Component"))
		{
			ImGui::SeparatorText("Components");
			if (ImGui::MenuItem("Mesh"))
			{
				m_SceneManager->GetCurrentScene()->AddComponent<MeshComponent>(inspectorID,
					m_SceneManager->GetAssetManager()->GetMesh("Assets/Models/planeobj.obj").get(),
					m_SceneManager->GetAssetManager()->GetShader("Assets/Shaders/SimpleShader/SimpleShaders.hlsl").get(),
					m_SceneManager->GetAssetManager()->GetMaterial("Assets/Materials/default.lmat"));
			}

			if(ImGui::MenuItem("Script"))
			{
				m_SceneManager->GetCurrentScene()->AddComponent<ScriptComponent>(inspectorID,
					m_SceneManager->GetAssetManager()->GetScript("Assets/Scripts/testing.lua"));
				m_SceneManager->GetCurrentScene()->GetECS()->GetObjectComponent<ScriptComponent>(inspectorID)->m_Script->m_ECS = m_SceneManager->GetCurrentScene()->GetECS();
			}

			if(ImGui::MenuItem("Physics"))
				m_SceneManager->GetCurrentScene()->AddComponent<PhysicsComponent>(inspectorID);
			if (ImGui::MenuItem("Collider"))
				m_SceneManager->GetCurrentScene()->AddComponent<ColliderComponent>(inspectorID);

			if (ImGui::MenuItem("Camera"))
				m_SceneManager->GetCurrentScene()->AddComponent<CameraComponent>(inspectorID, new PerspectiveCamera(), false, glm::vec4(0.1f, 0.1f, 0.1f, 1.0f), false);
			

			
			ImGui::EndMenu();
		}


		if (ImGui::MenuItem("Delete Object"))
			m_ObjectsToDelete.push_back({ id });

		ImGui::EndPopup();
	}

	if (nodeOpen) {
		for (SceneGraphNode* child : *node->getNodes())
			BuildHiearchyText(inspectorID, child, names, HasBeenRightClicked);
		ImGui::TreePop();
	}
}


void HierarchyPanel::EndFrame(unsigned int& inspectorID)
{
	// Process queued additions
	for (auto& [parent, name] : m_ObjectsToAdd)
	{
		unsigned int id = m_SceneManager->AddObject(parent, name);
	}
	m_ObjectsToAdd.clear();

	// Process queued deletes
	for (unsigned int id : m_ObjectsToDelete)
	{
		m_SceneManager->GetCurrentScene()->DestroyGameObject(id);
		if (inspectorID == id)
			inspectorID = 0;
	}
	m_ObjectsToDelete.clear();
}
