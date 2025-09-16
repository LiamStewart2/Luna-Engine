#include "InspectorPanel.h"

void InspectorPanel::Update(unsigned int& inspectorID)
{
	if(m_Show == false)
		return;
	{
		ImGui::Begin("Inspector", &m_Show);
		Scene* scene = m_SceneManager->GetCurrentScene();

		if (scene->GetECS()->HasComponent<NameComponent>(inspectorID))
		{
			ImGui::SeparatorText("Name");

			ImGui::InputText("##", &scene->GetECS()->GetObjectComponent<NameComponent>(inspectorID)->m_Name);
		}

		if (scene->GetECS()->HasComponent<Transform>(inspectorID))
		{
			ImGui::SeparatorText("Transform");
			Transform* transform = scene->GetECS()->GetObjectComponent<Transform>(inspectorID);

			ImGui::Text("Position");
			ImGui::InputFloat("position x", &transform->position.x);
			ImGui::InputFloat("position y", &transform->position.y);
			ImGui::InputFloat("position z", &transform->position.z);

			if (glm::quat(glm::radians(m_EulerDegrees)) != transform->rotation)
			{
				m_EulerDegrees = glm::degrees(transform->GetEulerRotation());
			}

			ImGui::Text("Rotation");
			ImGui::InputFloat("rotation x", &m_EulerDegrees.x);
			ImGui::InputFloat("rotation y", &m_EulerDegrees.y);
			ImGui::InputFloat("rotation z", &m_EulerDegrees.z);

			transform->SetEulerRotation(glm::radians(m_EulerDegrees));


			ImGui::Text("Scale");
			ImGui::InputFloat("scale x", &transform->scale.x);
			ImGui::InputFloat("scale y", &transform->scale.y);
			ImGui::InputFloat("scale z", &transform->scale.z);
		}

		if (scene->GetECS()->HasComponent<MeshComponent>(inspectorID))
		{
			ImGui::SeparatorText("Mesh");
			
			Mesh* mesh = scene->GetECS()->GetObjectComponent<MeshComponent>(inspectorID)->mesh;
			ImGui::Columns(2, "Mesh", false);

			ImGui::Text("Mesh:"); 
			ImGui::NextColumn();
			if (ImGui::ImageButton(mesh->path.c_str(), m_ModelIcon->ID, ImVec2{96.0f, 96.0f}))
			{
				std::string fpath = FileNavigation::OpenFileDialog({
					{L"Mesh Files", L"*.obj"},
					{L"All Files", L"*.*"}
					}, 1);
				if (!fpath.empty())
					scene->GetECS()->GetObjectComponent<MeshComponent>(inspectorID)->mesh = m_SceneManager->GetAssetManager()->GetMesh(fpath).get();
			}

			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM_MODEL"))
				{
					const char* path = (const char*)payload->Data;
					scene->GetECS()->GetObjectComponent<MeshComponent>(inspectorID)->mesh = m_SceneManager->GetAssetManager()->GetMesh(path).get();
				}
				ImGui::EndDragDropTarget();
			}

			ImGui::NextColumn();

			Texture* texture = scene->GetECS()->GetObjectComponent<MeshComponent>(inspectorID)->texture;
			ImGui::Columns(2, "Texture", false);
			ImGui::Text("Texture:");
			ImGui::NextColumn();
			if (ImGui::ImageButton(texture->path.c_str(), texture->ID, ImVec2{ 96.0f, 96.0f }))
			{
				std::string fpath = FileNavigation::OpenFileDialog({
					{L"Texture Files", L"*.png;*.jpg;*.jpeg"},
					{L"All Files", L"*.*"}
					}, 1);
				if (!fpath.empty())
					scene->GetECS()->GetObjectComponent<MeshComponent>(inspectorID)->texture = m_SceneManager->GetAssetManager()->GetTexture(fpath).get();
			}

			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM_TEXTURE"))
				{
					const char* path = (const char*)payload->Data;
					scene->GetECS()->GetObjectComponent<MeshComponent>(inspectorID)->texture = m_SceneManager->GetAssetManager()->GetTexture(path).get();
				}
				ImGui::EndDragDropTarget();
			}
		}
		if (scene->GetECS()->HasComponent<CameraComponent>(inspectorID))
		{
			ImGui::SeparatorText("Camera");

			ImGui::ColorEdit4("Background Colour", glm::value_ptr(scene->GetECS()->GetObjectComponent<CameraComponent>(inspectorID)->m_Camera->m_EditorBackgroundColour));

			ImGui::Checkbox("Main Camera", &scene->GetECS()->GetObjectComponent<CameraComponent>(inspectorID)->m_MainCamera);
		}

		if (scene->GetECS()->HasComponent<LightComponent>(inspectorID))
		{
			ImGui::SeparatorText("Light");

			LightComponent* component = scene->GetECS()->GetObjectComponent<LightComponent>(inspectorID);

			float* lightColor = new float[3] {component->m_LightColor.x, component->m_LightColor.y, component->m_LightColor.z};
			ImGui::InputFloat3("Light Color", lightColor);
			component->m_LightColor = glm::vec3(lightColor[0], lightColor[1], lightColor[2]);
		}

		ImGui::End();
	}
}