#include "ReworkedRenderer.h"

namespace Luna
{
	std::shared_ptr<IRendererAPI> ReworkedRenderer::s_RendererAPI = IRendererAPI::Create();;

	void ReworkedRenderer::Init(std::shared_ptr<RendererContext> renderContext)
	{
		s_RendererAPI->Init(renderContext);
	}
	void ReworkedRenderer::Shutdown()
	{
	}
	void ReworkedRenderer::BeginFrame(SceneManager* sceneManager, IFramebuffer* framebuffer, ObjectTransformPairing<Camera>* camera)
	{
		//s_RendererAPI->Clear();
		s_RendererAPI->StartFrame(sceneManager,framebuffer, camera);
	}
	void ReworkedRenderer::EndFrame(SceneManager* sceneManager, IFramebuffer* framebuffer)
	{
		s_RendererAPI->EndFrame(sceneManager, framebuffer);
	}
	void ReworkedRenderer::Render(SceneManager* sceneManager, IFramebuffer* framebuffer)
	{
		// BIND EVERYTHING HERE

		Transform transform = Transform(0, glm::vec3(0, 0, 0));
		//transform.transformMatrix = glm::rotate(glm::mat4(1.0f), (float)glfwGetTime(), glm::vec3(0, 1, 0));
		//transform.transformMatrix = glm::mat4(1);

		s_RendererAPI->RenderIndexed(12 * 3, &transform);
	}
}