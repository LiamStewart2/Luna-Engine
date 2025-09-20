// Luna Engine - AssetManager

#pragma once

#include <string>
#include <unordered_map>
#include <memory>

#include "AssetLoader.h"

#include "../Renderer/Texture.h"
#include "../Renderer/Mesh.h"
#include "../Renderer/Shader.h"
#include "../Scripting/Script.h"

/*
	Uses Luna Engine - AssetLoader to load and manage buffers of all assets used in the lifetime of an application

	ToDo: For each asset type, check the file extension and use corresponding functions to make loading assets smoother
*/
class AssetManager
{
public:
	AssetManager();
	~AssetManager();

	// std::shared_ptr<Texture> GetTexture - Loads a texture from filepath and returns a reference to the object from the buffer
	// std::string filepath - the file path of the image file
	std::shared_ptr<Texture> GetTexture(std::string filepath);
	
	// std::shared_ptr<Mesh> GetMesh - Loads a mesh from filepath and returns a reference to the object from the buffer
	// std::string filepath - the file path of the mesh file
	std::shared_ptr<Mesh> GetMesh(std::string filepath);
	
	// std::shared_ptr<Shader> GetTexture - Loads a shader from folder and returns a reference to the object from the buffer
	// std::string filepath - the file path of the shader folder
	std::shared_ptr<Shader> GetShader(std::string filepath);

	// std::shared_ptr<Script> GetScript - Loads a script and returns a reference to the object from the buffer
	// std::string filepath - the file path of the script file
	std::shared_ptr<Script> GetScript(std::string filepath);

private:
	std::unordered_map<std::string, std::shared_ptr<Texture>> m_Textures;
	std::unordered_map<std::string, std::shared_ptr<Mesh>> m_Meshes;
	std::unordered_map<std::string, std::shared_ptr<Shader>> m_Shaders;
	std::unordered_map<std::string, std::shared_ptr<Script>> m_Scripts;
};

