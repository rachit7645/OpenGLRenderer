#include "Files.h"

std::string directory = "";

void Files::SetResourceDirectory(const std::string& relPath)
{
	directory = std::filesystem::absolute(relPath).u8string();
}

const std::string& Files::GetResourceDirectory()
{
	return directory;
}