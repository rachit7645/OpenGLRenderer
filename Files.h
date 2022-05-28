#ifndef FILES_H
#define FILES_H

#include <string>
#include <filesystem>

#include "Util.h"

namespace Files
{
	void SetResourceDirectory(const std::string& relPath);
	const std::string& GetResourceDirectory();
}

#endif