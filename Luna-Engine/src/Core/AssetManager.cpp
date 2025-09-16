// Luna Engine - AssetManager

/*
	Uses Luna Engine - AssetLoader to load and manage buffers of all assets used in the lifetime of an application

	ToDo: For each asset type, check the file extension and use corresponding functions to make loading assets smoother
*/

#include "AssetManager.h"

AssetManager::AssetManager()
{
}

AssetManager::~AssetManager()
{
}

// std::shared_ptr<Texture> GetTexture - Loads a texture from filepath and returns a reference to the object from the buffer
// std::string filepath - the file path of the image file

std::shared_ptr<Texture> AssetManager::GetTexture(std::string filepath)
{
	if(m_Textures.find(filepath) == m_Textures.end())
	{

		std::cout << filepath << std::endl;
		std::shared_ptr<Texture> texture = m_Textures[filepath] = std::make_shared<Texture>();
		AssetLoader::LoadTexture(texture, filepath.c_str());
	}
	return m_Textures[filepath];
}

// std::shared_ptr<Mesh> GetMesh - Loads a mesh from filepath and returns a reference to the object from the buffer
// std::string filepath - the file path of the mesh file

std::shared_ptr<Mesh> AssetManager::GetMesh(std::string filepath)
{
	if (m_Meshes.find(filepath) == m_Meshes.end())
	{

		std::cout << filepath << std::endl;
		std::shared_ptr<Mesh> mesh = m_Meshes[filepath] = std::make_shared<Mesh>();
		AssetLoader::LoadMeshOBJ(mesh, filepath.c_str());
	}
	return m_Meshes[filepath];
}

// std::shared_ptr<Shader> GetTexture - Loads a shader from folder and returns a reference to the object from the buffer
// std::string filepath - the file path of the shader folder

std::shared_ptr<Shader> AssetManager::GetShader(std::string filepath)
{
	if (m_Shaders.find(filepath) == m_Shaders.end())
	{
		std::cout << filepath << std::endl;
		m_Shaders[filepath] = std::make_shared<Shader>(filepath.c_str(), (filepath + std::string("/shader.vs")).c_str(), (filepath + std::string("/shader.fs")).c_str(), (filepath + std::string("/shader.gs")).c_str());
	}
	return m_Shaders[filepath];
}
