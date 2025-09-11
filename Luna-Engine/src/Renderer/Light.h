#pragma once

#include "GLM/glm.hpp"

#include "LightManager.h"
#include "../Core/Globals.h"
#include "Shader.h"
#include "Mesh.h"
#include "../ECS/TransformComponent.h"

class Light
{
public:
	Light() {}
	~Light() {}

	void BuildLight(LightManager* lightManager);
	void RenderObjectToDepthmap(Mesh* mesh, Transform* transform, Shader* depthmapShader);

	void BindTexture(Shader* shader);
	void FrameSetup(LightManager* lightManager, Camera* camera, Transform* cameraTransform, Transform* lightTransform, Shader* shader, FrameBuffer* framebuffer);
	void FrameReset();


	unsigned int SHADOW_WIDTH = 2048, SHADOW_HEIGHT = 2048;

	unsigned int m_DepthmapFBO = 0;
	unsigned int m_MatricesUBO = 0;
	unsigned int m_DepthmapsTextureID = 0;

	glm::mat4 lightSpaceMatrix = glm::mat4(1);
private:
};