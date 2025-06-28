#include "Light.h"

Light::Light(glm::vec3 _position, glm::vec3 _direction, glm::vec3 _color) : position(_position), direction(_direction), color(_color)
{

}

Light::~Light()
{
}

void Light::BuildLight(Shader* depthmapShader)
{
}

void Light::RenderObjectToDepthmap(Mesh* mesh, Transform* transform, Shader* depthmapShader)
{
}

void Light::BindDepthmapTexture(Shader* shader)
{
}
