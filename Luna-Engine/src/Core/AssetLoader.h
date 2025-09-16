// Luna Engine - Asset Loader

#pragma once

#include <iostream>

#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <array>

#include "../Renderer/Mesh.h"
#include "../Renderer/Texture.h"

/*
Collection of file loading methods for various object types currently including
 - OBJ to mesh loading
 - stb_image supported files loading to texture
*/
class AssetLoader
{
public:
	// void LoadTexture - Loads a texture from any stb_image supported file
	// Supports both RGB and RGBA channelled images
	// std::shared_ptr<Texture> texture - a reference to the texture object the data should be loaded into
	// const char* filepath - the file path of the image file
	static void LoadTexture(std::shared_ptr<Texture> texture, const char* filepath);

	// void LoadMeshOBJ - Loads a mesh object from an OBJ file
	// OBJ files must be triangulated and only contain one mesh
	// std::shared_ptr<Mesh> mesh - a reference to the mesh object the data should be loaded into
	// const char* filepath - the file path of the .obj file. currently does not check if file extension is .obj
	static void LoadMeshOBJ(std::shared_ptr<Mesh> mesh, const char* filepath);
};

