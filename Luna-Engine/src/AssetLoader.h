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
	static void LoadMesh(Mesh& mesh, const char* filepath);
};

