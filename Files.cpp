#include "Files.h"

std::string resDir = "";

void Files::SetResourceDirectory(const std::string& relPath)
{
	resDir = std::filesystem::absolute(relPath).u8string();
}

const std::string& Files::GetResourceDirectory()
{
	return resDir;
}

std::string Files::GetName(const std::string& path)
{
	return std::filesystem::path(path).filename().u8string();
}

std::string Files::GetRelative(const std::string& path)
{
	return std::filesystem::relative(path, resDir).u8string();
}