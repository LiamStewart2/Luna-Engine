#include "Light.h"

Light::Light(glm::vec3 _position, glm::vec3 _direction, glm::vec3 _color) : position(_position), direction(_direction), color(_color)
{

}

Light::~Light()
{
}

void Light::BuildLight(LightManager* lightManager)
{
    // Create 3D Texture Arrays
    glEnable(GL_DEPTH_TEST);

	glGenFramebuffers(1, &depthmapFBO);
	glGenTextures(1, &depthmapsTextureID);
    glBindTexture(GL_TEXTURE_2D_ARRAY, depthmapsTextureID);
    glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_DEPTH_COMPONENT32F, SHADOW_WIDTH, SHADOW_HEIGHT, int(lightManager->GetShadowCascadePlanes().size()) + 1, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);

    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

    float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
    glTexParameterfv(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_BORDER_COLOR, borderColor);

    glBindFramebuffer(GL_FRAMEBUFFER, depthmapFBO);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthmapsTextureID, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);

    int status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (status != GL_FRAMEBUFFER_COMPLETE)
        std::cerr << "[ERROR] Framebuffer is not complete" << std::endl;

    glBindFramebuffer(GL_FRAMEBUFFER, 0);  

    // Configure UBO

    glGenBuffers(1, &matricesUBO);
    glBindBuffer(GL_UNIFORM_BUFFER, matricesUBO);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::mat4x4) * 16, nullptr, GL_STATIC_DRAW);
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, matricesUBO);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void Light::RenderObjectToDepthmap(Mesh* mesh, Transform* transform, Shader* depthmapShader)
{

    glCullFace(GL_FRONT);
    depthmapShader->BindShader();
    mesh->BindMesh();

    glm::mat4 model = glm::mat4(1.0f);

    model = glm::translate(model, transform->position);

    model = glm::rotate(model, glm::radians(transform->rotation.x), { 1, 0, 0 });
    model = glm::rotate(model, glm::radians(transform->rotation.y), { 0, 1, 0 });
    model = glm::rotate(model, glm::radians(transform->rotation.z), { 0, 0, 1 });

    model = glm::scale(model, transform->scale);

    depthmapShader->SetMat4("model", model);

    glDrawElements(GL_TRIANGLES, mesh->indices.size(), GL_UNSIGNED_INT, 0);
    glCullFace(GL_BACK);
}

void Light::BindTexture(Shader* shader)
{
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D_ARRAY, depthmapsTextureID);
}

void Light::FrameSetup(LightManager* lightManager, Shader* depthmapShader, Shader* shader)
{
    glm::mat4 lightProjection, lightView;
    glm::mat4 lightSpaceMatrix;
    float near_plane = 0.1f, far_plane = 150.0f;
    lightProjection = glm::ortho(-30.0f, 30.0f, -30.0f, 30.0f, near_plane, far_plane);

    lightView = glm::lookAt(position, position + glm::normalize(direction), glm::vec3(0, 1, 0));

    lightSpaceMatrix = lightProjection * lightView;

    std::vector<glm::mat4> matrices = lightManager->GenerateLightSpaceMatrices(direction);
    glBindBuffer(GL_UNIFORM_BUFFER, matricesUBO);
    for (size_t i = 0; i < matrices.size(); i++)
        glBufferSubData(GL_UNIFORM_BUFFER, i * sizeof(glm::mat4x4), sizeof(glm::mat4x4), &matrices[i]);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    shader->BindShader();
    shader->SetInt("cascadeCount", lightManager->GetShadowCascadePlanes().size());
    for(size_t i = 0; i < lightManager->GetShadowCascadePlanes().size(); i++)
        shader->SetFloat("cascadePlaneDistances[" + std::to_string(i) + "]", lightManager->GetShadowCascadePlanes()[i]);

    glBindFramebuffer(GL_FRAMEBUFFER, depthmapFBO);
    glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
    glClear(GL_DEPTH_BUFFER_BIT);
}

void Light::FrameReset()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}