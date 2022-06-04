#ifndef CUBE_MAP_H
#define CUBE_MAP_H

#include <string>
#include <vector>
#include <GL/glew.h>

#include "Files.h"
#include "Util.h"
#include "Log.h"
#include "Texture.h"

namespace Renderer
{
	class CubeMap
	{
	public:
		CubeMap(const std::vector<std::string>& files);
		~CubeMap();

		GLuint id;
	};
}

#endif