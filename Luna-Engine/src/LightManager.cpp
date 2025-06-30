#include "LightManager.h"

glm::mat4 LightManager::GenerateLightSpaceMatrix(const glm::vec3& lightPosition, const glm::vec3& lightDirection)
{
	std::vector<glm::vec4> corners = GetFrustumCornersWorldSpace();
	for (int i = 0; i < corners.size(); i++)
	{
		for (int j = 0; j < 4; j++)
			std::cout << corners[i][j] << " ";
		std::cout << std::endl;
	}
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