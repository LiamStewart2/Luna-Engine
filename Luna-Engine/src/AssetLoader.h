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
	static void LoadTexture(Texture& texture, const char* filepath);

	static void LoadMeshOBJ(Mesh& mesh, const char* filepath);
};

