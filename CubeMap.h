#ifndef CUBE_MAP_H
#define CUBE_MAP_H

#include <array>
#include <GL/glew.h>

namespace Renderer
{
	class CubeMap
	{
	public:
        // Default constructor
        CubeMap() = default;
		// Generate Cubemap texture
		explicit CubeMap(const std::array<const char*, 6>& files);
        // Destructor
		~CubeMap();

		GLuint id = 0;
	};
}

#endif