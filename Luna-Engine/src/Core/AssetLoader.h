#pragma once

#include <iostream>

#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <array>

#include "Mesh.h"
#include "Texture.h"

class AssetLoader
{
public:
	static void LoadTexture(std::shared_ptr<Texture> texture, const char* filepath);

	static void LoadMeshOBJ(std::shared_ptr<Mesh> mesh, const char* filepath);
};

