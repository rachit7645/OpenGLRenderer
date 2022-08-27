#ifndef FILES_H
#define FILES_H

#include <string_view>

namespace Files
{
	void SetResourceDirectory(const std::string_view relPath);
	const std::string& GetResourceDirectory();

	std::string GetName(const std::string_view path);
	std::string GetRelative(const std::string_view path);
}

#endif