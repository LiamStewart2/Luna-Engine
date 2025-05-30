#pragma once

#include <iostream>

#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <array>

#include "Mesh.h"

class AssetLoader
{
public:
	static void LoadMeshOBJ(Mesh& mesh, const char* filepath);
};

