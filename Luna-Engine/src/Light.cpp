#include "Light.h"

Light::Light(glm::vec3 _position, glm::vec3 _direction, glm::vec3 _color) : position(_position), direction(_direction), color(_color)
{

}

Light::~Light()
{
}

void Light::BuildLight(Shader* depthmapShader)
{
	glGenFramebuffers(1, &depthmapFBO);
	glGenTextures(1, &depthmapTextureID);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glBindFramebuffer(GL_FRAMEBUFFER, depthmapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthmapTextureID);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Light::RenderObjectToDepthmap(Mesh* mesh, Transform* transform, Shader* depthmapShader)
{
}

void Light::BindDepthmapTexture(Shader* shader)
{
}
