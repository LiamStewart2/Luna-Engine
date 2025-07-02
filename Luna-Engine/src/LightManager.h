#pragma once

#include <vector>

#include "GLM/glm.hpp"

#include "Camera.h"

class LightManager
{
public:
	LightManager(Camera* _camera) : camera(_camera) {}
	~LightManager() {}

	std::vector<glm::mat4>  GenerateLightSpaceMatrices(const glm::vec3& lightDirection);
	glm::mat4 GenerateLightSpaceMatrix(const float& nearPlane, const float& farPlane, const glm::vec3& lightDirection);

	std::vector<float>& GetShadowCascadePlanes() {return shadowCascadeLevels;}

	Camera* camera;
	std::vector<float> shadowCascadeLevels = {camera->farPlane / 50.0f, camera->farPlane / 25.0f, camera->farPlane / 10.0f, camera->farPlane / 2.0f};

	std::vector<glm::vec4> GetFrustumCornersWorldSpace(const float& nearPlane, const float& farPlane);
	glm::vec3 GetCenterOfPoints(const std::vector<glm::vec4>& corners);
	glm::mat4 GetLightProjection(std::vector<glm::vec4>& corners, const glm::mat4& lightView);
private:
};

