#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "../Core/Globals.h"

#include "../ECS/ECS.h"
#include "../Core/Camera.h"

#include "Light.h"
#include "Material.h"
#include "Mesh.h"
#include "Shader.h"

template <class T>
struct ObjectTransformPairing
{
	T* object;
	Transform* objectTransform;
};

class Renderer {
public:
	Renderer();
	~Renderer();
	
	void RenderPass(
		ObjectTransformPairing<Camera> camera,
		ObjectTransformPairing<LightComponent> light,

		std::unordered_map<unsigned int, Transform*> transforms,
		std::unordered_map<unsigned int, MeshComponent*> meshComponents,

		LightManager* lightManager, 
		Shader* shader, Shader* depthMapShader
	);

	void SetShaderFrame( 
		ObjectTransformPairing<Camera>& camera, 
		ObjectTransformPairing<LightComponent>& light, 

		Shader* depthmapShader, Shader* shader
	);

	void RenderObject(Transform* transform, LightComponent* light, Mesh* mesh, Texture* texture, Material* material, Shader* shader);
};