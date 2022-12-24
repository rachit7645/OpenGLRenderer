#ifndef FILES_H
#define FILES_H

#include <string_view>
#include <string>

namespace Engine
{
	class Files
	{
	private:
		// Default constructor
		Files() = default;
	public:
		// Set resource directory
		void SetResources(const std::string_view relPath);
		// Get resource directory
		const std::string& GetResources();
		// Get file name from path
		std::string GetName(const std::string_view path);
		// Get directory from path
		std::string GetDirectory(const std::string_view path);
	private:
		// Resource directory
		std::string m_resDir;
	public:
		// Get instance
		static Files& GetInstance();
	};
}

#endif