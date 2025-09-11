#include "Renderer.h"

Renderer::Renderer()
{
}

Renderer::~Renderer()
{
}

void Renderer::RenderSceneFromMainCamera(ObjectTransformPairing<Camera> camera, ObjectTransformPairing<LightComponent> light, std::unordered_map<unsigned int, Transform*> transforms, 
	std::unordered_map<unsigned int, MeshComponent*> meshComponents, LightManager* lightManager, 
	Shader* shader, Shader* depthMapShader, FrameBuffer* framebuffer)
{
	if (camera.object == nullptr) { std::cout << "NO CAMERA PASSED" << std::endl; return; }
	if (shader == nullptr) { std::cout << "NO SHADER PASSED" << std::endl; return; }
	if (framebuffer == nullptr) { std::cout << "NO FRAMEBUFFER PASSED" << std::endl; }

	
	lightManager->InitCascadeLevels(camera.object);
	light.object->m_Light.FrameSetup(lightManager, camera.object, camera.objectTransform, light.objectTransform, shader, framebuffer);
	for (auto& [id, meshComponent] : meshComponents)
	{
		auto transformIt = transforms.find(meshComponent->gameObject);
		if (transformIt != transforms.end())
			light.object->m_Light.RenderObjectToDepthmap(meshComponent->mesh, transformIt->second, depthMapShader);
	}
	light.object->m_Light.FrameReset();

	SetShaderFrame(camera, light, depthMapShader, shader, framebuffer);
	for (auto& [id, meshComponent] : meshComponents)
	{
		auto transformIt = transforms.find(meshComponent->gameObject);
		if (transformIt != transforms.end())
		{
			RenderObject(transformIt->second, light.object, meshComponent->mesh, meshComponent->texture, meshComponent->material, meshComponent->shader);
		}
	}

	if (framebuffer != nullptr)
		framebuffer->Unbind();
}

void Renderer::RenderObject(Transform* transform, LightComponent* light, Mesh* mesh, Texture* texture, Material* material, Shader* shader)
{
	shader->BindShader();
	texture->BindTexture(shader);
	mesh->BindMesh();
	light->m_Light.BindTexture(shader);

	glm::mat4 model = transform->transformMatrix;

	shader->SetMat4("model", model);

	shader->SetVec3("oMaterial.color", material->color);

	glDrawElements(GL_TRIANGLES, mesh->indices.size(), GL_UNSIGNED_INT, 0);
}

void Renderer::SetShaderFrame(ObjectTransformPairing<Camera>& camera, ObjectTransformPairing<LightComponent>& light, Shader* depthmapShader, Shader* shader, FrameBuffer* framebuffer)
{
	if (framebuffer != nullptr)
		framebuffer->Bind();
	else
		glViewport(0, 0, SCREEN_WIDTH, SCREEN_WIDTH);

	glm::vec4 bgCol = camera.object->m_EditorBackgroundColour;
	glClearColor(bgCol.x, bgCol.y, bgCol.z, bgCol.w);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	shader->BindShader();

	glm::mat4 projection = camera.object->GetProjection();
	glm::mat4 view = camera.object->GetView(camera.objectTransform);

	shader->SetMat4("projection", projection);
	shader->SetMat4("view", view);

	if(light.object != nullptr && light.objectTransform != nullptr)
	{
		light.object->m_Light.BindTexture(shader);
		shader->SetVec3("oLight.position", light.objectTransform->position);
		shader->SetVec3("oLight.direction", light.objectTransform->Forward());
		shader->SetVec3("oLight.color", light.object->m_LightColor);
	}

	shader->SetFloat("farPlane", camera.object->m_FarPlane);

	shader->SetVec3("oViewPosition", camera.objectTransform->position);

	std::vector<float> shadowCascadeLevels = { camera.object->m_FarPlane / 50.0f, camera.object->m_FarPlane / 25.0f, camera.object->m_FarPlane / 10.0f, camera.object->m_FarPlane / 2.0f };
	shader->SetInt("cascadeCount", shadowCascadeLevels.size());
	for (size_t i = 0; i < shadowCascadeLevels.size(); ++i)
	{
		shader->SetFloat("cascadePlaneDistances[" + std::to_string(i) + "]", shadowCascadeLevels[i]);
	}
}