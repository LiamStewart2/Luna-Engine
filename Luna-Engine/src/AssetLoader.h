#pragma once

#include <iostream>

#include <stdio.h>
#include <fstream>
#include <string>

#include "Mesh.h"

class AssetLoader
{
public:
	static void LoadMesh(Mesh& mesh, const char* filepath);
};

