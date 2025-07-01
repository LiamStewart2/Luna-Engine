#include "LightManager.h"

glm::mat4 LightManager::GenerateLightSpaceMatrix(const glm::vec3& lightPosition, const glm::vec3& lightDirection)
{
	std::vector<glm::vec4> corners = GetFrustumCornersWorldSpace();
	glm::vec3 center = GetCenterOfPoints(corners);

	glm::mat4 lightView = glm::lookAt(center + glm::normalize(lightDirection), center, glm::vec3(0, 1.0f, 0.0f));
	glm::mat4 lightProjection = GetLightProjection(corners, lightView);

	return lightProjection * lightView;
}

std::vector<glm::vec4> LightManager::GetFrustumCornersWorldSpace()
{
	glm::mat4 inverse = glm::inverse(camera->GetProjection() * camera->GetViewMatrix());

	std::vector<glm::vec4> frustumCorners;
	for (int x = 0; x < 2; x++)
	{
		for (int y = 0; y < 2; y++)
		{
			for (int z = 0; z < 2; z++)
			{
				glm::vec4 point = inverse * glm::vec4(
					2.0f * x - 1.0f,
					2.0f * y - 1.0f,
					2.0f * z - 1.0f,
					1.0f);
				frustumCorners.push_back(point / point.w);
			}
		}
	}
	return frustumCorners;
}

glm::vec3 LightManager::GetCenterOfPoints(const std::vector<glm::vec4>& corners)
{
	glm::vec3 center = glm::vec3(0, 0, 0);
	for (const glm::vec4& point : corners)
		center +=  glm::vec3(point);
	center /= corners.size();
	return center;
}


glm::mat4 LightManager::GetLightProjection(std::vector<glm::vec4>& corners, const glm::mat4& lightView)
{
	float minX = std::numeric_limits<float>::max();
	float maxX = std::numeric_limits<float>::lowest();
	float minY = std::numeric_limits<float>::max();
	float maxY = std::numeric_limits<float>::lowest();
	float minZ = std::numeric_limits<float>::max();
	float maxZ = std::numeric_limits<float>::lowest();

	for (const glm::vec4& v : corners)
	{
		const glm::vec4 trf = lightView * v;
		minX = std::min(minX, trf.x);
		maxX = std::min(maxX, trf.x);
		minY = std::min(minY, trf.y);
		maxY = std::min(maxY, trf.y);
		minZ = std::min(minZ, trf.z);
		maxZ = std::min(maxZ, trf.z);
	}

	constexpr float zMult = 10.0f;

	if(minZ < 0)
		minZ *= zMult;
	else
		minZ /= zMult;

	if(maxZ < 0)
		maxZ /= zMult;
	else
		maxZ *= zMult;

	return glm::ortho(minX, maxX, minY, maxY, minZ, maxZ);
}