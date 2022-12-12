#ifndef FILES_H
#define FILES_H

#include <string_view>
#include <string>

namespace Files
{
	class FileHandler
	{
	private:
		// Default constructor
		FileHandler() = default;
	public:
		// Set resource directory
		void SetResourceDirectory(const std::string_view relPath);
		// Get resource directory
		const std::string& GetResourceDirectory();
		// Get file name from path
		std::string GetName(const std::string_view path);
		// Get directory from path
		std::string GetDirectory(const std::string_view path);
	private:
		// Resource directory
		std::string m_resDir;
	public:
		// Get instance
		static FileHandler& GetInstance();
	};
}

#endif