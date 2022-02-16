#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>
#include <GL/glew.h>
#include "Util.h"

namespace Renderer
{
	struct Texture
	{
		u32 textureID;
		int width;
		int height;
		int channels;
	}; 
	Texture LoadTexture(const std::string& path);
}
#endif // TEXTURE_H