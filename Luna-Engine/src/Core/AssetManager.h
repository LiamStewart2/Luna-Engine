// Luna Engine - AssetManager

#pragma once

#include <string>
#include <unordered_map>
#include <memory>

#include "AssetLoader.h"
#include "JSON/json.hpp"


#include "../Renderer/ITexture.h"
#include "../Renderer/IMesh.h"
#include "../Renderer/IShader.h"
#include "../Renderer/Material.h"
#include "../Scripting/Script.h"

/*
	Uses Luna Engine - AssetLoader to load and manage buffers of all assets used in the lifetime of an application

	ToDo: For each asset type, check the file extension and use corresponding functions to make loading assets smoother
*/
namespace Luna
{
	class AssetManager
	{
	public:
		AssetManager();
		~AssetManager();

		// std::shared_ptr<Texture> GetTexture - Loads a texture from filepath and returns a reference to the object from the buffer
		// std::string filepath - the file path of the image file
		std::shared_ptr<ITexture> GetTexture(std::string filepath);

		// std::shared_ptr<Mesh> GetMesh - Loads a mesh from filepath and returns a reference to the object from the buffer
		// std::string filepath - the file path of the mesh file
		std::shared_ptr<IMesh> GetMesh(std::string filepath);

		// std::shared_ptr<Shader> GetTexture - Loads a shader from folder and returns a reference to the object from the buffer
		// std::string filepath - the file path of the shader folder
		std::shared_ptr<IShader> GetShader(std::string filepath);


		std::shared_ptr<Material> GetMaterial(std::string filepath);


		// std::shared_ptr<Script> GetScript - Loads a script and returns a reference to the object from the buffer
		// std::string filepath - the file path of the script file
		std::shared_ptr<Script> GetScript(std::string filepath);


	private:
		std::unordered_map<std::string, std::shared_ptr<ITexture>> m_Textures;
		std::unordered_map<std::string, std::shared_ptr<IMesh>> m_Meshes;
		std::unordered_map<std::string, std::shared_ptr<Material>> m_Materials;
		std::unordered_map<std::string, std::shared_ptr<IShader>> m_Shaders;
		std::unordered_map<std::string, std::shared_ptr<Script>> m_Scripts;
	};
}

