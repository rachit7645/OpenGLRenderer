#include "Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

using namespace Renderer;

// TODO: Please fix this path hell

Texture::Texture(const std::string& path, PathType pathType)
{
	std::string newPath;

	if (pathType == RELATIVE)
	{
		LOG_INFO("Loading texture: ", path, "\n");
		newPath = Files::GetResourceDirectory() + path;
	}
	else if (pathType == ABSOLUTE)
	{
		LOG_INFO("Loading texture: ", Files::GetRelative(path), "\n");
		newPath = path;
	}

	u8* data = stbi_load(newPath.c_str(), &width, &height, &channels, STBI_rgb_alpha);
	if (data == nullptr)
	{
		LOG_ERROR("Unable to load texture: ", path);
	}

	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_LOD_BIAS, TEXTURE_LOD_BIAS);

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