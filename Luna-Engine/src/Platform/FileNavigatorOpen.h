#pragma once

#include <string>
#include <vector>

struct FileTypeFilter {
	std::wstring name;
	std::wstring extensions;
};

class FileNavigatorOpen
{
public:
	std::string OpenFileDialog(const std::vector<FileTypeFilter>& filters, int defaultIndex = 1);
};

