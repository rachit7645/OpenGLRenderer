#include "Texture.h"

// This needs to be in a .cpp file so that it works (why tho?)
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

using namespace Renderer;

Texture::Texture(const std::string &path, PathType pathType)
{
	// FIXME: Paths from assimp can be weird
#ifdef _DEBUG
	std::string newPath = path;
	if (pathType == PathType::RELATIVE)
	{
		newPath = "../" + path;
	}
	u8 *data = stbi_load(newPath.c_str(), &width, &height, &channels, 4);
#else
	u8 *data = stbi_load(path.c_str(), &width, &height, &channels, 4);
#endif

	if (data == nullptr)
	{
		Logger::LogAndExit("Unable to open file: " + path, TEXTURE_LOAD_FAILED);
	}

	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_LOD_BIAS, LOD_BIAS);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);

	stbi_image_free(data);
}

Texture::~Texture()
{
	glDeleteTextures(1, &id);
}