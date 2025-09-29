#include "GamePanel.h"

void GamePanel::UpdateGame(unsigned int& inspectorID, FrameBuffer* framebuffer, bool& runtime)
{
	if (m_Show == false)
		return;

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
	ImGui::Begin("Game");

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
	ImGui::Image(framebuffer->GetAttatchmentID(), imageSize, ImVec2{0, 1}, ImVec2{1, 0});

	// Play Game Button

	ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 12.5f);
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1, 1, 1, 0.1f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(1, 1, 1, 0.9f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(1.0f, 1.0f, 1.0f, 0.6f));

	ImGui::SetCursorPos(ImVec2(viewportSize.x / 2 - 12.5, 25));
	if(!runtime) {
		if (ImGui::ImageButton("PlayGame", m_PlayIcon->ID, ImVec2(50, 50)))
			runtime = !runtime;
	} else {
		if (ImGui::ImageButton("StopGame", m_StopIcon->ID, ImVec2(50, 50)))
			runtime = !runtime;
	}

	ImGui::PopStyleVar();
	ImGui::PopStyleColor(3);

	ImGui::End();
	ImGui::PopStyleVar();
}
