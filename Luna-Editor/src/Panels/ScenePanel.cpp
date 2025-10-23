#include "ScenePanel.h"
#include <d3d11_4.h>

void ScenePanel::UpdateScene(unsigned int& inspectorID, std::shared_ptr<Luna::IFramebuffer> sceneFramebuffer, std::vector<std::pair<ACTIONS, std::string>>* actions)
{
	if(m_Show == false)
		return;

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
	ImGui::Begin("Scene", &m_Show);

	// Get viewport size (content region inside the window)
	ImVec2 viewportPos = ImGui::GetWindowPos();
	ImVec2 viewportSize = ImGui::GetContentRegionAvail();

	// Center the framebuffer image in the window
	float aspectRatio = (float)16 / (float)9;

	ImVec2 imageSize{
		viewportSize.x,
		viewportSize.x / aspectRatio
	};
	if (viewportSize.y * aspectRatio < viewportSize.x)
	{
		imageSize.x = viewportSize.y * aspectRatio;
		imageSize.y = viewportSize.y;
	}

	ImVec2 imageOffset{
		(viewportSize.x - imageSize.x) * 0.5f,
		(viewportSize.y - imageSize.y) * 0.5f
	};

	ID3D11ShaderResourceView* srv = (ID3D11ShaderResourceView*)sceneFramebuffer->GetColorAttachment();
	if (LunaWindow::m_FocusedWindow->GetKey(GLFW_KEY_1))
	{
		srv = (ID3D11ShaderResourceView*)sceneFramebuffer->GetDepthAttachment();
	}

	ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPos().x + imageOffset.x, ImGui::GetCursorPos().y + imageOffset.y));
	ImGui::Image((ImTextureRef)srv, imageSize, ImVec2{ 0, 0 }, ImVec2{ 1, 1 });
	EditorCamera::sceneWindowHovered = ImGui::IsItemHovered();

	if (ImGui::BeginDragDropTarget())
	{
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM_SCENE"))
		{
			const char* path = (const char*)payload->Data;
			actions->push_back({LOADSCENE, std::string(path)});
		}
		ImGui::EndDragDropTarget();
	}
}

void ScenePanel::UpdateGizmos(unsigned int& inspectorID, ObjectTransformPairing<Camera>& camera)
{
	// Prepare ImGuizmo
	ImVec2 topLeft = ImGui::GetItemRectMin();
	ImVec2 size = ImGui::GetItemRectSize();
	ImGuizmo::SetRect(topLeft.x, topLeft.y, size.x, size.y);

	glm::mat4 view = camera.object->GetView(camera.objectTransform);
	glm::mat4 proj = camera.object->GetProjection();

	if (inspectorID != 0)
	{
		ImGuizmo::SetOrthographic(false);
		ImGuizmo::SetDrawlist();

		// Handle Input for ImGuizmo Operation Selection
		if (ImGui::IsKeyPressed(ImGuiKey_T))
			m_CurrentOperation = ImGuizmo::TRANSLATE;
		else if (ImGui::IsKeyPressed(ImGuiKey_R))
			m_CurrentOperation = ImGuizmo::ROTATE;
		else if (ImGui::IsKeyPressed(ImGuiKey_E))
			m_CurrentOperation = ImGuizmo::SCALE;

		Transform* objectTransform = m_SceneManager->GetCurrentScene()->GetECS()->GetObjectComponent<Transform>(inspectorID);
		glm::mat4 matrix = objectTransform->transformMatrix;

		ImGuizmo::Manipulate(glm::value_ptr(view), glm::value_ptr(proj),
			m_CurrentOperation, ImGuizmo::WORLD, glm::value_ptr(matrix));

		if (ImGuizmo::IsUsing())
		{
			objectTransform->SetComponentsFromMatrix(matrix);
		}
	}

	EditorCamera::sceneWindowFocused = ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows);
	ImGui::End();
	ImGui::PopStyleVar();
}
