#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "../Core/Globals.h"

#include "../ECS/ECS.h"
#include "../Core/Camera.h"

#include "Light.h"
#include "Material.h"
#include "Mesh.h"
#include "Shader.h"
#include "FrameBuffer.h"

class Renderer {
public:
	Renderer();
	~Renderer();
	
	void RenderSceneFromMainCamera(
		EntityComponentSystem* ECS, 
		LightManager* lightManager, 
		Shader* shader, Shader* depthMapShader, 
		FrameBuffer* framebuffer
	);

	void EditorRenderPass(
		EntityComponentSystem* ECS,
		LightManager* lightManager, 
		EditorCamera* camera,
		Shader* shader, Shader* depthMapShader,
		FrameBuffer* framebuffer
	);

	void SetShaderFrame(
		EntityComponentSystem* ECS, 
		Camera* camera, Transform* cameraTransform, 
		Shader* depthmapShader, Shader* shader,
		FrameBuffer* framebuffer
	);

	void RenderObject(Transform* transform, LightComponent* light, Mesh* mesh, Texture* texture, Material* material, Shader* shader);
};