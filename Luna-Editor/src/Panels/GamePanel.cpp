#include "GamePanel.h"

void GamePanel::UpdateGame(unsigned int& inspectorID, FrameBuffer* framebuffer)
{
	if (m_Show == false)
		return;

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
	ImGui::Begin("Game", &m_Show);

	// Get viewport size (content region inside the window)
	ImVec2 viewportPos = ImGui::GetWindowPos();
	ImVec2 viewportSize = ImGui::GetContentRegionAvail();

	// Center the framebuffer image in the window
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

	ImGui::End();
	ImGui::PopStyleVar();
}
