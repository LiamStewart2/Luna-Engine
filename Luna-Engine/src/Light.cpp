#include "Light.h"

Light::Light(glm::vec3 _position, glm::vec3 _direction, glm::vec3 _color) : position(_position), direction(_direction), color(_color)
{

}

Light::~Light()
{
}

void Light::BuildLight()
{
    glEnable(GL_DEPTH_TEST);
	glGenFramebuffers(1, &depthmapFBO);
	glGenTextures(1, &depthmapTextureID);
    glBindTexture(GL_TEXTURE_2D, depthmapTextureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

    glBindFramebuffer(GL_FRAMEBUFFER, depthmapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthmapTextureID, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);  
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
    shader->SetInt("shadowMap", 1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, depthmapTextureID);
}

void Light::FrameSetup(Shader* depthmapShader, Shader* shader)
{
    glm::mat4 lightProjection, lightView;
    glm::mat4 lightSpaceMatrix;
    float near_plane = 0.1f, far_plane = 150.0f;
    lightProjection = glm::ortho(-30.0f, 30.0f, -30.0f, 30.0f, near_plane, far_plane);
    
    lightView = glm::lookAt(position, position + glm::normalize(direction), glm::vec3(0, 1, 0));
    lightSpaceMatrix = lightProjection * lightView;

    depthmapShader->BindShader();
    depthmapShader->SetMat4("lightSpaceMatrix", lightSpaceMatrix);
    shader->BindShader();
    shader->SetMat4("lightSpaceMatrix", lightSpaceMatrix);

    glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
    glBindFramebuffer(GL_FRAMEBUFFER, depthmapFBO);
    glClear(GL_DEPTH_BUFFER_BIT);
}

void Light::FrameReset()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}