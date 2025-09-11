#pragma once

#include <iostream>

#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <array>

#include "../Renderer/Mesh.h"
#include "../Renderer/Texture.h"

class AssetLoader
{
public:
	static void LoadTexture(std::shared_ptr<Texture> texture, const char* filepath);

	static void LoadMeshOBJ(std::shared_ptr<Mesh> mesh, const char* filepath);
};

