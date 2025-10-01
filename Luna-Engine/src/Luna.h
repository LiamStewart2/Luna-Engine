
// ENGINE CORE

#include "Core/SceneManager.h"
#include "Core/AssetManager.h"
#include "Core/Camera.h"
#include "Core/TransformationManager.h"
#include "ECS/ECS.h"

#if defined(_WIN32)
// WINDOWS UTILS
#include "Platform/Windows/FileNavigation.h"

// DIRECTX11
//#include "Platform/DirectX11"
#endif

// RENDERER

#include "Renderer/Renderer.h"
#include "Renderer-2/ReworkedRenderer.h"
#include "Renderer-2/RendererContext.h"

// SCRIPTING

#include "Scripting/ScriptManager.h"