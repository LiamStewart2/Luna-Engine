#include "ImGuiLayer.h"

void ImGuiLayer::Init()
{
	m_MainScale = ImGui_ImplGlfw_GetContentScaleForMonitor(glfwGetPrimaryMonitor());

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

	ImGuiStyle& style = ImGui::GetStyle();
	style.ScaleAllSizes(m_MainScale);
	style.FontScaleDpi = m_MainScale;
	ImGui::StyleColorsDark();

	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}

	ImGui_ImplGlfw_InitForOpenGL(m_Window->GetHandle(), true);
	const char* glsl_version = "#version 460";
	ImGui_ImplOpenGL3_Init(glsl_version);
}

void ImGuiLayer::StartFrame()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	ImGuizmo::BeginFrame();
}

static const float identityMatrix[16] =
{ 1.f, 0.f, 0.f, 0.f,
	0.f, 1.f, 0.f, 0.f,
	0.f, 0.f, 1.f, 0.f,
	0.f, 0.f, 0.f, 1.f };

// DOCKING IMPLEMENATION FROM THE CHERNO USING IMGUI DOCKING BRANCH
void ImGuiLayer::Update()
{
	StartFrame();

	Scene* scene = m_SceneManager->GetCurrentScene();

	static bool dockspaceOpen = true;
	static bool opt_fullscreen_persistant = true;
	bool opt_fullscreen = opt_fullscreen_persistant;
	static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

	// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
	// because it would be confusing to have two docking targets within each others.
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
	if (opt_fullscreen)
	{
		ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->Pos);
		ImGui::SetNextWindowSize(viewport->Size);
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
	}

	if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
		window_flags |= ImGuiWindowFlags_NoBackground;

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);
	ImGui::PopStyleVar();

	if (opt_fullscreen)
		ImGui::PopStyleVar(2);

	// DockSpace
	ImGuiIO& io = ImGui::GetIO();
	ImGuiStyle& style = ImGui::GetStyle();
	float minWinSizeX = style.WindowMinSize.x;
	style.WindowMinSize.x = 250.0f;
	style.WindowMinSize.y = 250.0f;
	if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
	{
		ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
	}

	style.WindowMinSize.x = minWinSizeX;

	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{

			ImGui::EndMenu();
		}


		if (ImGui::BeginMenu("View"))
		{
			if(ImGui::MenuItem("Scene"))
				m_ShowScene = true;
			if (ImGui::MenuItem("Inspector"))
				m_ShowInspector = true;
			if (ImGui::MenuItem("Show File"))
				m_ShowFile = true;
			if (ImGui::MenuItem("Hiearchy"))
				m_ShowHierachy = true;
			ImGui::EndMenu();
		}

		ImGui::EndMenuBar();
	}

	if(m_ShowInspector){
		ImGui::Begin("Inspector", &m_ShowInspector);

		if(scene->GetECS()->HasComponent<NameComponent>(m_CurrentInspectorGameObject))
		{
			ImGui::SeparatorText("Name");

			ImGui::InputText("##", &scene->GetECS()->GetObjectComponent<NameComponent>(m_CurrentInspectorGameObject)->m_Name);
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
			if (ImGui::Button(scene->GetECS()->GetObjectComponent<MeshComponent>(m_CurrentInspectorGameObject)->mesh->path.c_str()))
			{
				FileNavigatorOpen filenav;
				std::string fpath = filenav.OpenFileDialog({
					{L"Mesh Files", L"*.obj"},
					{L"All Files", L"*.*"}
				}, 1);
				if (!fpath.empty())
					scene->GetECS()->GetObjectComponent<MeshComponent>(m_CurrentInspectorGameObject)->mesh = m_AssetManager->GetMesh(fpath).get();
			}
				
			if (ImGui::Button(scene->GetECS()->GetObjectComponent<MeshComponent>(m_CurrentInspectorGameObject)->texture->path.c_str()))
			{
				FileNavigatorOpen filenav;
				std::string fpath = filenav.OpenFileDialog({
					{L"Texture Files", L"*.png;*.jpg"},
					{L"All Files", L"*.*"}
				}, 1);
				if(!fpath.empty())
					scene->GetECS()->GetObjectComponent<MeshComponent>(m_CurrentInspectorGameObject)->texture = m_AssetManager->GetTexture(fpath).get();
			}
		}

		if (scene->GetECS()->HasComponent<CameraComponent>(m_CurrentInspectorGameObject))
		{
			ImGui::SeparatorText("Camera");

			ImGui::Checkbox("Main Camera", &scene->GetECS()->GetObjectComponent<CameraComponent>(m_CurrentInspectorGameObject)->m_MainCamera);
		}

		if (scene->GetECS()->HasComponent<LightComponent>(m_CurrentInspectorGameObject))
		{
			ImGui::SeparatorText("Light");

			LightComponent* component = scene->GetECS()->GetObjectComponent<LightComponent>(m_CurrentInspectorGameObject);

			float* lightColor = new float[3] {component->m_LightColor.x, component->m_LightColor.y, component->m_LightColor.z};
			ImGui::InputFloat3("Light Color", lightColor);
			component->m_LightColor = glm::vec3(lightColor[0], lightColor[1], lightColor[2]);
		}

		ImGui::End();
	}

	if(m_ShowHierachy){
		ImGui::Begin("Hieararcy", &m_ShowHierachy);

		std::unordered_map<unsigned int, NameComponent*> names = scene->GetECS()->GetAllComponentsOfType<NameComponent>();
		BuildHiearchyText(scene->GetSceneGraph(), &names);

		ImGui::End();
	}

	if(m_ShowFile){
		ImGui::Begin("File", &m_ShowFile);

		ImGui::InputText("filepath: ", filepathForScene, 50);
		if(ImGui::Button("Save Scene"))
			m_SceneManager->SaveCurrentScene(filepathForScene);

		ImGui::End();
	}

	if (m_ShowScene)
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
		ImGui::Begin("Scene", &m_ShowScene);

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

		// Prepare ImGuizmo
		ImGuizmo::SetRect(viewportPos.x + imageOffset.x, viewportPos.y + imageOffset.y, imageSize.x, imageSize.y);

		EditorCamera* editorCamera = &m_SceneManager->GetCurrentScene()->camera;
		glm::mat4 view = editorCamera->GetView();
		glm::mat4 proj = editorCamera->GetProjection();

		if (m_CurrentInspectorGameObject != 0)
		{
			ImGuizmo::SetOrthographic(false);
			ImGuizmo::SetDrawlist();


			// Handle Input for ImGuizmo Operation Selection
			if(ImGui::IsKeyPressed(ImGuiKey_T))
				m_CurrentOperation = ImGuizmo::TRANSLATE;
			else if (ImGui::IsKeyPressed(ImGuiKey_R))
				m_CurrentOperation = ImGuizmo::ROTATE;
			else if (ImGui::IsKeyPressed(ImGuiKey_S))
				m_CurrentOperation = ImGuizmo::SCALE;

			Transform* objectTransform = m_SceneManager->GetCurrentScene()->GetECS()->GetObjectComponent<Transform>(m_CurrentInspectorGameObject);
			glm::mat4 matrix = objectTransform->transformMatrix;



			ImGuizmo::Manipulate(glm::value_ptr(view), glm::value_ptr(proj),
				m_CurrentOperation, ImGuizmo::LOCAL, glm::value_ptr(matrix));

			if (ImGuizmo::IsUsing())
			{
				glm::vec3 translation, rotation, scale;
				ImGuizmo::DecomposeMatrixToComponents(glm::value_ptr(matrix),
					glm::value_ptr(translation),
					glm::value_ptr(rotation),
					glm::value_ptr(scale));

				switch (m_CurrentOperation)
				{
				case ImGuizmo::TRANSLATE:
					objectTransform->position = translation;
					break;
				case ImGuizmo::ROTATE:
					objectTransform->rotation = rotation;
					break;
				case ImGuizmo::SCALE:
					objectTransform->scale = scale;
					break;
				default:
					break;
				}
			}
		}

		EditorCamera::sceneWindowFocused = ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows);
		EditorCamera::sceneWindowHovered = ImGui::IsWindowHovered(ImGuiHoveredFlags_RootAndChildWindows);

		ImGui::End();
		ImGui::PopStyleVar();
	}

	ImGui::End();

	//ImGui::ShowDemoWindow();

	for (auto& [parent, name] : m_ObjectsToAdd)
	{
		unsigned int id = m_SceneManager->AddObject(parent, name);
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
		{
			m_ObjectsToAdd.push_back({ id, "New Object" });
		}
		
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

	if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		GLFWwindow* backup_current_context = glfwGetCurrentContext();
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		glfwMakeContextCurrent(backup_current_context);
	}
}
