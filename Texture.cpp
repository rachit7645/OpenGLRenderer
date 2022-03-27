#include "Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

using namespace Renderer;

// Loads a texture into memory, then an OpenGL object
Texture::Texture(const std::string& path)
{
	#ifdef _DEBUG
		std::string newPath = "../" + path;
		u8* data = stbi_load(newPath.c_str(), &width, &height, &channels, 4);
	#else
		u8* data = stbi_load(path.c_str(), &width, &height, &channels, 4);
	#endif
	assert(data != NULL);

	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_LOD_BIAS, LOD_BIAS);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1); 
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(data);
}