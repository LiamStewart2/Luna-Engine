#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Globals.h"

#include "ECS/ECS.h"
#include "ECS/TransformComponent.h"
#include "ECS/CameraComponent.h"
#include "ECS/LightComponent.h"

#include "Light.h"
#include "Material.h"
#include "Mesh.h"
#include "Shader.h"
#include "Camera.h"
#include "FrameBuffer.h"
#include "EditorCamera.h"

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