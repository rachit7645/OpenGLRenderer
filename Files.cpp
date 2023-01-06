#include "Files.h"

#include <filesystem>

// Using namespaces
using namespace Engine;

// Namespace alias
namespace filesystem = std::filesystem;

void Files::SetResources(const std::string_view relPath)
{
	// Set resource path
	m_resDir = filesystem::absolute(relPath).u8string();
}

const std::string& Files::GetResources()
{
	// Return resource path
	return m_resDir;
}

std::string Files::GetName(const std::string_view path)
{
	// Return name
	return filesystem::path(path).filename().u8string();
}

std::string Files::GetDirectory(const std::string_view path)
{
	// Get directory
	auto directory = filesystem::path(path).parent_path().u8string();
	// Get separator
	auto separator = static_cast<char>(filesystem::path::preferred_separator);
	// Add and return
	return directory + separator;
}

Files& Files::GetInstance()
{
	// Static storage
	static Files files;
	// Return
	return files;
}