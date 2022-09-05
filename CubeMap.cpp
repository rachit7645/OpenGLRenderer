#include "CubeMap.h"
#include "stb/stb_image.h"

#include "Files.h"
#include "Util.h"
#include "Log.h"
#include "Texture.h"

using namespace Renderer;

CubeMap::CubeMap(const std::array<const char*, 6>& files)
{
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_CUBE_MAP, id);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S,     GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T,     GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R,     GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_LOD_BIAS,   TEXTURE_LOD_BIAS);

	for (usize i = 0; i < files.size(); ++i)
	{
		int width, height, channels;
		u8* data = stbi_load
		(
			(Files::GetResourceDirectory() + files[i]).c_str(),
			&width,
			&height,
			&channels,
			STBI_rgb_alpha	
		);

		if (data == nullptr)
		{
			LOG_ERROR("Unable to open file: ", files[i]);
		}

		glTexImage2D
		(
			GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
			0,
			GL_RGBA,
			width,
			height,
			0,
			GL_RGBA,
			GL_UNSIGNED_BYTE,
			data
		);
		
		stbi_image_free(data);
	}
	
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

CubeMap::~CubeMap()
{
	glDeleteTextures(1, &id);
}