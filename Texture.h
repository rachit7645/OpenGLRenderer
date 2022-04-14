#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>
#include <map>

#include <GL/glew.h>
#include "Util.h"

namespace Renderer
{
	constexpr auto LOD_BIAS = -0.5f;

	struct Texture
	{
		// Loads a texture into memory, then an OpenGL object
		Texture(const std::string &path);
		~Texture();

		u32 id;
		int width;
		int height;
		int channels;
	};
}
#endif // TEXTURE_H