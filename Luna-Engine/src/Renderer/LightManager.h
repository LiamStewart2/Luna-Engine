#pragma once

#include <vector>

#include "GLM/glm.hpp"

#include "../Core/Camera.h"
#include "FrameBuffer.h"

class LightManager
{
public:
	LightManager() {}
	~LightManager() {}

	void InitCascadeLevels(Camera* _camera)
	{
		shadowCascadeLevels = { _camera->m_FarPlane / 50.0f, _camera->m_FarPlane / 25.0f, _camera->m_FarPlane / 10.0f, _camera->m_FarPlane / 2.0f };
	}
	
	std::vector<glm::mat4>  GenerateLightSpaceMatrices(Camera* camera, Transform* cameraTransform, FrameBuffer* framebuffer, const glm::vec3& lightDirection);
	glm::mat4 GenerateLightSpaceMatrix(Camera* camera, Transform* cameraTransform, FrameBuffer* framebuffer, const float& nearPlane, const float& farPlane, const glm::vec3& lightDirection);

	std::vector<float>& GetShadowCascadePlanes() {return shadowCascadeLevels;}

	std::vector<float> shadowCascadeLevels;

	std::vector<glm::vec4> GetFrustumCornersWorldSpace(Camera* camera, Transform* cameraTransform, FrameBuffer* framebuffer, const float& nearPlane, const float& farPlane);
	glm::vec3 GetCenterOfPoints(const std::vector<glm::vec4>& corners);
	glm::mat4 GetLightProjection(std::vector<glm::vec4>& corners, const glm::mat4& lightView);
private:
};