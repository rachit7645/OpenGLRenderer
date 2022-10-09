#include "Files.h"

#include <filesystem>

std::string m_resDir;

void Files::SetResourceDirectory(const std::string_view relPath)
{
	m_resDir = std::filesystem::absolute(relPath).u8string();
}

const std::string& Files::GetResourceDirectory()
{
	return m_resDir;
}

std::string Files::GetName(const std::string_view path)
{
	return std::filesystem::path(path).filename().u8string();
}

std::string Files::GetRelative(const std::string_view path)
{
	return std::filesystem::relative(path, m_resDir).u8string();
}