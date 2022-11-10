#ifndef CUBE_MAP_H
#define CUBE_MAP_H

#include <array>
#include <string_view>
#include <GL/glew.h>

#include "Texture.h"

namespace Renderer
{
	class CubeMap : public Texture
	{
	public:
		// Empty constructor
		CubeMap();
		// Main constructor
		explicit CubeMap(const std::array<const std::string_view, 6>& files);
	};
}

#endif