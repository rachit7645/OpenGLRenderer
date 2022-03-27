#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>
#include <GL/glew.h>
#include "Util.h"

namespace Renderer
{
	constexpr f32 LOD_BIAS = -0.5f;
	
	struct Texture
	{
		Texture(const std::string& path);
		u32 textureID;
		int width;
		int height;
		int channels;
	}; 
}
#endif // TEXTURE_H