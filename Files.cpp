#include "Files.h"

#include <filesystem>

// Using namespaces
using namespace Files;

// Namespace alias
namespace filesystem = std::filesystem;

void FileHandler::SetResourceDirectory(const std::string_view relPath)
{
	m_resDir = filesystem::absolute(relPath).u8string();
}

const std::string& FileHandler::GetResourceDirectory()
{
	return m_resDir;
}

std::string FileHandler::GetName(const std::string_view path)
{
	return filesystem::path(path).filename().u8string();
}

std::string FileHandler::GetDirectory(const std::string_view path)
{
	// Get directory
	auto directory = filesystem::path(path).parent_path().u8string();
	// Get separator
	auto separator = static_cast<char>(filesystem::path::preferred_separator);
	// Add
	return directory + separator;
}

FileHandler& FileHandler::GetInstance()
{
	// Static storage
	static FileHandler files;
	// Return
	return files;
}