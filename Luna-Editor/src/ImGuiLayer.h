#pragma once

#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_glfw.h"
#include "imgui/backends/imgui_impl_opengl3.h"
#include "imgui/misc/cpp/imgui_stdlib.h"
#include "imgui/ImGuizmo.h"
#include "GLM/gtx/matrix_decompose.hpp"

#include "Luna.h"
#include "EditorCamera.h"

#include <math.h>
#include <unordered_map>

enum ACTIONS
{
	LOADSCENE
};

class ImGuiLayer
{
public:
	ImGuiLayer(LunaWindow* window = nullptr, AssetManager* assetManager = nullptr, SceneManager* sceneManager = nullptr) : m_Window(window), m_AssetManager(assetManager), m_SceneManager(sceneManager)
	{
		if(window != nullptr && m_AssetManager != nullptr && m_SceneManager != nullptr)
			Init();
	}
	~ImGuiLayer() {}

	void Init();
	
	void StartFrame();
	void Update(ObjectTransformPairing<Camera>& camera);

	void Render();
private:
	LunaWindow* m_Window = nullptr;
	SceneManager* m_SceneManager = nullptr;
	AssetManager* m_AssetManager = nullptr;

	float m_MainScale = 0;
	bool m_ShowAddObject = true;

	bool m_ShowScene = true;
	bool m_ShowGame = true;
	bool m_ShowHierachy = true;
	bool m_ShowContentBrowser = true;
	bool m_ShowInspector = true;

	bool m_SaveShortcutUsed = false;

	unsigned int NameCharacterLimit = 100;
	float Indentation = 10.0f;

	int m_ParentObject = 0;
	char* m_Name = new char[NameCharacterLimit]();
	float* m_PlacementPosition = new float[3] {0, 0, 0};
	float* m_RotationPlacement = new float[3] {0, 0, 0};
	float* m_ScalePlacement = new float[3] {1, 1, 1};

	char* filepathForScene = new char[MAX_OBJECT_NAME_SIZE]("");

	int m_CurrentInspectorGameObject = 0;

	int m_ParentObjectID = 0;
	char* m_ObjectName = new char[MAX_OBJECT_NAME_SIZE]("");

	unsigned int m_RenamingGameObject = 0;
	char m_RenameBuffer[128] = "";

	int m_ObjectToRemoveID = 0;

	ImGuizmo::OPERATION m_CurrentOperation = ImGuizmo::TRANSLATE;

	std::vector<std::pair<ACTIONS, std::string>> m_Actions;
	std::vector<unsigned int> m_ObjectsToDelete;
	std::vector<std::pair<unsigned int, std::string>> m_ObjectsToAdd;

	void BuildHiearchyText(SceneGraphNode* node, std::unordered_map<unsigned int, NameComponent*>* names, bool& HasBeenRightClicked);
};