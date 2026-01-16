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

		if (scene->GetECS()->HasComponent<ScriptComponent>(inspectorID))
		{
			ImGui::SeparatorText("Script");

			ScriptComponent* component = scene->GetECS()->GetObjectComponent<ScriptComponent>(inspectorID);
			
			ImGui::Columns(2, "Script", false);

			ImGui::Text("Script:");
			ImGui::NextColumn();
			if (ImGui::ImageButton(component->m_Script->GetFilepath().c_str(), m_CodeIcon->GetTextureReference(), ImVec2{96.0f, 96.0f}))
			{
				std::string fpath = FileNavigation::OpenFileDialog({
					{L"Script Files", L"*.lua"},
					{L"All Files", L"*.*"}
					}, 1);
				if (!fpath.empty())
				{
					scene->GetECS()->GetObjectComponent<ScriptComponent>(inspectorID)->m_Script = m_SceneManager->GetAssetManager()->GetScript(fpath);
					scene->GetECS()->GetObjectComponent<ScriptComponent>(inspectorID)->m_Script->m_ECS = scene->GetECS();
				}
			}

			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM_SCRIPT"))
				{
					const char* path = (const char*)payload->Data;
					scene->GetECS()->GetObjectComponent<ScriptComponent>(inspectorID)->m_Script = m_SceneManager->GetAssetManager()->GetScript(path);
					scene->GetECS()->GetObjectComponent<ScriptComponent>(inspectorID)->m_Script->m_ECS = scene->GetECS();
				}
				ImGui::EndDragDropTarget();
			}
			ImGui::TextWrapped(component->m_Script->GetFilepath().c_str());
			ImGui::Columns(1);
		}

		if (scene->GetECS()->HasComponent<PhysicsComponent>(inspectorID))
		{
			PhysicsComponent* component = scene->GetECS()->GetObjectComponent<PhysicsComponent>(inspectorID);

			ImGui::SeparatorText("Physics");

			ImGui::Checkbox("Simulate", &component->m_Simulate);

			ImGui::InputFloat("Gravity Scale", &component->m_GravityValue);
			ImGui::InputFloat("Mass", &component->m_Mass);
		}

		if (scene->GetECS()->HasComponent<ColliderComponent>(inspectorID))
		{
			ColliderComponent* component = scene->GetECS()->GetObjectComponent<ColliderComponent>(inspectorID);

			ImGui::SeparatorText("Collider");

			const char* items[] = {"Sphere", "Rect"};
			const char* comboPreviewValue = items[component->m_Shape];

			if (ImGui::BeginCombo("Collider Shape", comboPreviewValue, 0))
			{
				for (int n = 0; n < IM_ARRAYSIZE(items); n++)
				{
					const bool isSelected = (component->m_Shape == n);
					if(ImGui::Selectable(items[n], isSelected))
						component->m_Shape = (ColliderShape)n;
					if(isSelected)
						ImGui::SetItemDefaultFocus();
				}
				ImGui::EndCombo();
			}

			if(component->m_Shape == ColliderShape::Sphere)
			{
				ImGui::InputFloat("Collider Size", &component->m_ColliderSize.x);
			}

		}

		if (scene->GetECS()->HasComponent<MeshComponent>(inspectorID))
		{
			MeshComponent* component = scene->GetECS()->GetObjectComponent<MeshComponent>(inspectorID);

			ImGui::SeparatorText("Mesh");
			ImGui::Columns(2, "Mesh", false);

			ImGui::Text("Mesh:"); 
			ImGui::NextColumn();
			if (ImGui::ImageButton(component->mesh->m_Path.c_str(), m_ModelIcon->GetTextureReference(), ImVec2{96.0f, 96.0f}))
			{
				std::string fpath = FileNavigation::OpenFileDialog({
					{L"Mesh Files", L"*.obj"},
					{L"All Files", L"*.*"}
					}, 1);
				if (!fpath.empty())
					component->mesh = m_SceneManager->GetAssetManager()->GetMesh(fpath).get();
			}

			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM_MODEL"))
				{
					const char* path = (const char*)payload->Data;
					component->mesh = m_SceneManager->GetAssetManager()->GetMesh(path).get();
				}
				ImGui::EndDragDropTarget();
			}

			ImGui::NextColumn();
			
			ImGui::Columns(2, "Material", false);
			ImGui::Text("Material:");
			ImGui::NextColumn();
			if (ImGui::ImageButton(component->material->m_Path.c_str(), m_ModelIcon->GetTextureReference(), ImVec2{96.0f, 96.0f}))
			{
				std::string fpath = FileNavigation::OpenFileDialog({
					{L"Texture Files", L"*.lmat"},
					{L"All Files", L"*.*"}
					}, 1);
				if (!fpath.empty())
					component->material = m_SceneManager->GetAssetManager()->GetMaterial(fpath);
			}

			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM_MATERIAL"))
				{
					const char* path = (const char*)payload->Data;
					component->material = m_SceneManager->GetAssetManager()->GetMaterial(path);
				}
				ImGui::EndDragDropTarget();
			}

			ImGui::NextColumn();
			ImGui::Columns(1);
		}
		if (scene->GetECS()->HasComponent<CameraComponent>(inspectorID))
		{
			CameraComponent* cameraComp = scene->GetECS()->GetObjectComponent<CameraComponent>(inspectorID);

			ImGui::SeparatorText("Camera");

			ImGui::Checkbox("Use Skybox", &cameraComp->m_UseSkybox);

			if (!cameraComp->m_UseSkybox)
			{
				ImGui::ColorEdit4("Background Colour", glm::value_ptr(cameraComp->m_CameraBackgroundColor));				
			}


			ImGui::Checkbox("Main Camera", &cameraComp->m_MainCamera);
		}

		if (scene->GetECS()->HasComponent<LightComponent>(inspectorID))
		{
			ImGui::SeparatorText("Light");

			LightComponent* component = scene->GetECS()->GetObjectComponent<LightComponent>(inspectorID);

			float* lightColor = new float[3] {component->m_Light.m_LightColour.x, component->m_Light.m_LightColour.y, component->m_Light.m_LightColour.z};
			ImGui::InputFloat3("Light Color", lightColor);
			component->m_Light.m_LightColour = glm::vec4(lightColor[0], lightColor[1], lightColor[2], 1);
		}

		ImGui::End();
	}
}