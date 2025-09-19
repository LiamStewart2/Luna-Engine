#include "Application.h"

#include <iostream>

Application::Application()
{
	Init();
	MainLoop();
}

Application::~Application()
{
	Terminate();
}

char* ReadBytes(const std::string& filepath, uint32_t* outSize)
{
	std::ifstream stream(filepath, std::ios::binary | std::ios::ate);

	// filepath invalid
	if (!stream)
		return nullptr;

	std::streampos end = stream.tellg();
	stream.seekg(0, std::ios::beg);
	uint32_t size = end - stream.tellg();

	// file empty
	if (size == 0)
		return nullptr;

	char* buffer = new char[size];
	stream.read((char*)buffer, size);
	stream.close();

	*outSize = size;
	return buffer;
}

MonoAssembly* LoadCShardAssembly(const std::string& assemblyPath)
{
	uint32_t fileSize = 0;
	char* fileData = ReadBytes(assemblyPath, &fileSize);

	// NOTE: We can't use this image for anything other than loading the assembly because this image doesn't have a reference to the assembly
	MonoImageOpenStatus status;
	MonoImage* image = mono_image_open_from_data_full(fileData, fileSize, 1, &status, 0);

	if (status != MONO_IMAGE_OK)
	{
		const char* errorMessage = mono_image_strerror(status);
		// Log some error message using the errorMessage data
		std::cout << errorMessage << std::endl;
		return nullptr;
	}

	MonoAssembly* assembly = mono_assembly_load_from_full(image, assemblyPath.c_str(), &status, 0);
	mono_image_close(image);

	// Don't forget to free the file data
	delete[] fileData;

	return assembly;
}

void PrintAssemblyTypes(MonoAssembly* assembly)
{
	MonoImage* image = mono_assembly_get_image(assembly);
	const MonoTableInfo* typeDefinitionsTable = mono_image_get_table_info(image, MONO_TABLE_TYPEDEF);
	int32_t numTypes = mono_table_info_get_rows(typeDefinitionsTable);

	for (int32_t i = 0; i < numTypes; i++)
	{
		uint32_t cols[MONO_TYPEDEF_SIZE];
		mono_metadata_decode_row(typeDefinitionsTable, i, cols, MONO_TYPEDEF_SIZE);

		const char* nameSpace = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAMESPACE]);
		const char* name = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAME]);

		printf("%s.%s\n", nameSpace, name);
	}
}

MonoClass* GetClassInAssembly(MonoAssembly* assembly, const char* namespaceName, const char* className)
{
	MonoImage* image = mono_assembly_get_image(assembly);
	if (image == nullptr)
	{
		std::cout << "NOOOO" << std::endl;
	}
	MonoClass* klass = mono_class_from_name(image, namespaceName, className);

	if (klass == nullptr)
	{
		std::cout << "Could not get class " << className << std::endl;
		return nullptr;
	}

	return klass;
}

int Application::Init()
{
	glfwInit();

	window = LunaWindow::NewWindow("Epic Game", SCREEN_WIDTH, SCREEN_HEIGHT);

	m_SceneFramebuffer = FrameBuffer(FramebufferSpecification(1920, 1080, std::vector<FramebufferTextureAttatchment>({ RGBA8, DEPTH })));
	m_SceneFramebuffer.Update();
	m_GameFramebuffer = FrameBuffer(FramebufferSpecification(1920, 1080, std::vector<FramebufferTextureAttatchment>({ RGBA8, DEPTH })));
	m_GameFramebuffer.Update();

	sceneManager.LoadNewScene("Assets/Scenes/template scene.json");

	imGuiLayer = ImGuiLayer(window, sceneManager.GetAssetManager(), &sceneManager);
	

	//// LUA EXAMPLE
	std::string cmd = "a = 7 + 11 + math.sin(23.88)";
	lua_State* L = luaL_newstate();

	luaL_openlibs(L);

	int r = luaL_dofile(L, "Assets/Scripts/testing.lua");

	if (r == LUA_OK)
	{
		lua_getglobal(L, "a");
		if (lua_isnumber(L, -1))
		{
			float a_in_cpp = (float)lua_tonumber(L, -1);
			std::cout << "a_in_cpp = " << a_in_cpp << std::endl;
		}
	}
	else
	{
		std::string errormsg = lua_tostring(L, -1);
		std::cout << errormsg << std::endl;
	}
	lua_close(L);


	//// C# mono example
	std::cout << "START OF C#" << std::endl;
	mono_set_assemblies_path("../Dependencies/64-bit/mono/lib/4.5");
	MonoDomain* rootDomain = mono_jit_init("LunaScriptRuntime");
	if (rootDomain == nullptr)
		return -1;

	MonoDomain* appDomain = mono_domain_create_appdomain((char*)"LunaAppDomain", nullptr);
	mono_domain_set(appDomain, true);

	MonoAssembly* assembly = LoadCShardAssembly("A:/C++/Luna-Engine/ScriptingSandbox/bin/Release/ScriptingSandbox.dll");
	PrintAssemblyTypes(assembly);

	// Getting and instantiating a class object
	MonoClass* testingClass = GetClassInAssembly(assembly, "", "CSharpTesting");
	if(testingClass == nullptr)
		return -1;

	MonoObject* classInstance = mono_object_new(appDomain, testingClass);

	if(classInstance == nullptr)
		return -1;

	mono_runtime_object_init(classInstance);


	// invoking class functions

	MonoMethod* method = mono_class_get_method_from_name(testingClass, "PrintFloatVar", 0);
	if(method == nullptr)
		return -1;

	MonoObject* exception = nullptr;
	mono_runtime_invoke(method, classInstance, nullptr, &exception);


	std::cout << "END OF C#" << std::endl;
	return 0;
}

void Application::Terminate()
{
	glfwTerminate();
}

void Application::MainLoop()
{
	double lastTime = glfwGetTime();
	int frameCount = 0;

	while (!window->ShouldClose())
	{
		double currentTime = glfwGetTime();
		frameCount++;
		if (currentTime - lastTime >= 1.0)
		{
			std::string title = "FPS: " + std::to_string(frameCount);
			window->SetNewTitle(title.c_str());

			frameCount = 0;
			lastTime = currentTime;
		}

		HandleInput();

		Update();
		
		Render();

		window->Update();
	}
	sceneManager.UnloadCurrentScene();
	LunaWindow::CloseWindow(window);
}

void Application::HandleInput()
{
	if(window->GetKey(GLFW_KEY_F1) == GLFW_PRESS)
		LunaWindow::CloseWindow(window);
}

void Application::Update()
{
	sceneManager.Update();

	Transform cameraTransform = Transform(0, editorCamera.m_Position, glm::quat(glm::radians(editorCamera.m_Rotation)));
	ObjectTransformPairing<Camera> cameraPair = { (Camera*)&editorCamera, &cameraTransform };
	imGuiLayer.Update(cameraPair, &m_SceneFramebuffer, &m_GameFramebuffer);

	editorCamera.Update();


}

void Application::Render()
{

	glClearColor(0.7f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	Transform cameraTransform = Transform(0, editorCamera.m_Position, glm::quat(glm::radians(editorCamera.m_Rotation)));
	ObjectTransformPairing<Camera> cameraPair = { (Camera*)&editorCamera, &cameraTransform };
	sceneManager.Render(&renderer, cameraPair, &m_SceneFramebuffer);

	cameraPair = {nullptr, nullptr};
	sceneManager.Render(&renderer, cameraPair, &m_GameFramebuffer);

	imGuiLayer.Render();
}
