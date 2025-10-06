#include "Light.h"

void Light::BuildLight(LightManager* lightManager)
{
    // Create 3D Texture Arrays
    glEnable(GL_DEPTH_TEST);

	glGenFramebuffers(1, &m_DepthmapFBO);
	glGenTextures(1, &m_DepthmapsTextureID);
    glBindTexture(GL_TEXTURE_2D_ARRAY, m_DepthmapsTextureID);
    glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_DEPTH_COMPONENT32F, SHADOW_WIDTH, SHADOW_HEIGHT, int(lightManager->GetShadowCascadePlanes().size()) + 1, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);

    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

    float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
    glTexParameterfv(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_BORDER_COLOR, borderColor);
    
    glBindFramebuffer(GL_FRAMEBUFFER, m_DepthmapFBO);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, m_DepthmapsTextureID, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);

    int status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (status != GL_FRAMEBUFFER_COMPLETE)
        std::cerr << "[ERROR] Framebuffer is not complete" << std::endl;

    glBindFramebuffer(GL_FRAMEBUFFER, 0);  

    // Configure UBO

    glGenBuffers(1, &m_MatricesUBO);
    glBindBuffer(GL_UNIFORM_BUFFER, m_MatricesUBO);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::mat4x4) * 16, nullptr, GL_STATIC_DRAW);
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, m_MatricesUBO);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void Light::RenderObjectToDepthmap(Mesh* mesh, Transform* transform, Shader* depthmapShader)
{

    //glCullFace(GL_FRONT);
    depthmapShader->BindShader();
    mesh->BindMesh();

    depthmapShader->SetMat4("model", transform->transformMatrix);

    glDrawElements(GL_TRIANGLES, mesh->indices.size(), GL_UNSIGNED_INT, 0);
    //glCullFace(GL_BACK);
}

void Light::BindTexture(Shader* shader)
{
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D_ARRAY, m_DepthmapsTextureID);
}

void Light::FrameSetup(LightManager* lightManager, Camera* camera, Transform* cameraTransform, Transform* lightTransform, Shader* shader)
{
    glEnable(GL_DEPTH_TEST);

    std::vector<glm::mat4> matrices = lightManager->GenerateLightSpaceMatrices(camera, cameraTransform, lightTransform->Forward());
    glBindBuffer(GL_UNIFORM_BUFFER, m_MatricesUBO);
    for (size_t i = 0; i < matrices.size(); i++)
        glBufferSubData(GL_UNIFORM_BUFFER, i * sizeof(glm::mat4x4), sizeof(glm::mat4x4), &matrices[i]);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    shader->BindShader();
    shader->SetInt("cascadeCount", lightManager->GetShadowCascadePlanes().size());
    for(size_t i = 0; i < lightManager->GetShadowCascadePlanes().size(); i++)
        shader->SetFloat("cascadePlaneDistances[" + std::to_string(i) + "]", lightManager->GetShadowCascadePlanes()[i]);

    glBindFramebuffer(GL_FRAMEBUFFER, m_DepthmapFBO);
    glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
    glClear(GL_DEPTH_BUFFER_BIT);
}

void Light::FrameReset()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}