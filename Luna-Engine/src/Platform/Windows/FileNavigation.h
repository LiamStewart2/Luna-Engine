#pragma once

#include <string>
#include <vector>

struct FileTypeFilter {
	std::wstring name;
	std::wstring extensions;
};

class FileNavigation
{
public:
	static std::string OpenFileDialog(const std::vector<FileTypeFilter>& filters, int defaultIndex = 1);
	static std::string SaveAsFileDialog(const std::vector<FileTypeFilter>& filters, int defaultIndex = 1);
};

