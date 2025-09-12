#include "ScenePanel.h"

void ScenePanel::Update(unsigned int& inspectorID)
{
	if(m_Show == false)
		return;

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
	ImGui::Begin("Scene", &m_Show);

	// Get viewport size (content region inside the window)
	ImVec2 viewportPos = ImGui::GetWindowPos();
	ImVec2 viewportSize = ImGui::GetContentRegionAvail();

	// Center the framebuffer image in the window
	FrameBuffer* framebuffer = m_SceneManager->GetFrameBuffer();
	const FramebufferSpecification* specs = framebuffer->GetSpecs();
	float aspectRatio = (float)specs->Width / (float)specs->Height;

	ImVec2 imageSize{
		std::min(viewportSize.y * aspectRatio, viewportSize.x),
		std::min(viewportSize.x / aspectRatio, viewportSize.y)
	};

	ImVec2 imageOffset{
		(viewportSize.x - imageSize.x) * 0.5f,
		(viewportSize.y - imageSize.y) * 0.5f
	};

	ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPos().x + imageOffset.x, ImGui::GetCursorPos().y + imageOffset.y));
	ImGui::Image(framebuffer->GetAttatchmentID(), imageSize, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
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
	EditorCamera::sceneWindowHovered = ImGui::IsWindowHovered(ImGuiHoveredFlags_RootAndChildWindows);

	ImGui::End();
	ImGui::PopStyleVar();
}
