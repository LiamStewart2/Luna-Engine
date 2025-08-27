#pragma once

#include <Windows.h>
#include <ShObjIdl.h>
#include <string>

class FileNavigatorOpen
{
public:
	~FileNavigatorOpen();
	std::string Open();
private:
	IFileDialog* m_FileDialog = NULL;
};

