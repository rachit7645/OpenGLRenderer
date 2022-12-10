#include "Files.h"

#include <filesystem>

// Namespace alias
namespace filesystem = std::filesystem;

std::string m_resDir;

void Files::SetResourceDirectory(const std::string_view relPath)
{
	m_resDir = filesystem::absolute(relPath).u8string();
}

const std::string& Files::GetResourceDirectory()
{
	return m_resDir;
}

std::string Files::GetName(const std::string_view path)
{
	return filesystem::path(path).filename().u8string();
}

std::string Files::GetDirectory(const std::string_view path)
{
	return filesystem::path(path).parent_path().u8string() +
	       static_cast<char>(filesystem::path::preferred_separator);
}