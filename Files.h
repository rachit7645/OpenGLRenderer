#ifndef FILES_H
#define FILES_H

#include <string>
#include <filesystem>

#include "Util.h"

namespace Files
{
	void SetResourceDirectory(const std::string& relPath);
	const std::string& GetResourceDirectory();

	// TODO: Maybe use string_view here, for performance?
	std::string GetName(const std::string& path);
	std::string GetRelative(const std::string& path);
}

#endif