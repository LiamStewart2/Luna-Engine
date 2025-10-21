
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

#include "Renderer/ReworkedRenderer.h"
#include "Renderer/RendererContext.h"
#include "Renderer/IFramebuffer.h"
#include "Renderer/IMesh.h"
#include "Renderer/IShader.h"
#include "Renderer/ITexture.h"
#include "Renderer/Light.h"
#include "Renderer/Material.h"

// SCRIPTING

#include "Scripting/ScriptManager.h"