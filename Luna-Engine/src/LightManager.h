#pragma once

#include <vector>

#include "GLM/glm.hpp"

#include "Camera.h"

class LightManager
{
public:
	LightManager(Camera* _camera) : camera(_camera) {}
	~LightManager() {}

	glm::mat4 GenerateLightSpaceMatrix(const glm::vec3& lightPosition, const glm::vec3& lightDirection);

private:
	Camera* camera;
	std::vector<float> shadowCascadeLevels = {camera->farPlane / 50.0f, camera->farPlane / 25.0f, camera->farPlane / 10.0f, camera->farPlane / 2.0f};

	std::vector<glm::vec4> GetFrustumCornersWorldSpace();
	glm::vec3 GetCenterOfPoints(const std::vector<glm::vec4>& corners);
	glm::mat4 GetLightProjection(std::vector<glm::vec4>& corners, const glm::mat4& lightView);
};

