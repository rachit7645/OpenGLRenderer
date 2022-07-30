#ifndef CUBE_MAP_H
#define CUBE_MAP_H

#include <string>
#include <vector>
#include <GL/glew.h>

namespace Renderer
{
	class CubeMap
	{
	public:
        // Default constructor
        CubeMap() = default;
        // TODO: Use std::array of std::string_view instead
		CubeMap(const std::vector<std::string>& files);
        // Destructor
		~CubeMap();

		GLuint id = 0;
	};
}

#endif