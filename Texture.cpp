#include "Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

#include "Log.h"
#include "Files.h"
#include "Util.h"

using namespace Renderer;

// TODO: Add view layer functionality

Texture::Texture(const std::string_view path)
{
	auto data = LoadImage(path);

	CreateTexture();
	Bind();

	SetParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	SetParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	SetParameter(GL_TEXTURE_WRAP_S,     GL_REPEAT);
	SetParameter(GL_TEXTURE_WRAP_T,     GL_REPEAT);
	SetParameter(GL_TEXTURE_LOD_BIAS,   TEXTURE_LOD_BIAS);

	SetPixelParameter(GL_UNPACK_ALIGNMENT, 1);
	LoadImageData(data, GL_RGB, GL_RGB, GL_UNSIGNED_BYTE);
	GenerateMipmaps();

	Unbind();
	stbi_image_free(data);
}

Texture::Texture(const std::array<const std::string_view, 6>& files)
{
	// Set internal data
	type = GL_TEXTURE_CUBE_MAP;

	// Create texture
	CreateTexture();
	// Bind it
	Bind();

	// Set parameters
	SetParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	SetParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	SetParameter(GL_TEXTURE_WRAP_S,     GL_CLAMP_TO_EDGE);
	SetParameter(GL_TEXTURE_WRAP_T,     GL_CLAMP_TO_EDGE);
	SetParameter(GL_TEXTURE_WRAP_R,     GL_CLAMP_TO_EDGE);
	SetParameter(GL_TEXTURE_LOD_BIAS,   TEXTURE_LOD_BIAS);
	// Set pixel parameters
	SetPixelParameter(GL_UNPACK_ALIGNMENT, 1);

	for (usize i = 0; i < files.size(); ++i)
	{
		// Load data
		f32* data = LoadImageHDR(files[i]);
		// Upload to VRAM
		LoadImageData
		(
			reinterpret_cast<u8*>(data),
			GL_RGB16F,
			GL_RGB,
			GL_FLOAT,
			GL_TEXTURE_CUBE_MAP_POSITIVE_X + i
		);
		// Free memory
		stbi_image_free(data);
	}

	// Unbind
	Unbind();
}

u8* Texture::LoadImage(const std::string_view path)
{
	LOG_INFO("Loading texture: {}\n", path);

	u8* data = stbi_load
	(
		(Files::GetResourceDirectory() + path.data()).c_str(),
		&width,
		&height,
		&channels,
		STBI_rgb
	);

	if (data == nullptr)
	{
		LOG_ERROR("Unable to load texture: {}\n", path);
	}

	return data;
}

f32* Texture::LoadImageHDR(const std::string_view path)
{
	LOG_INFO("Loading texture: {}\n", path);

	f32* data = stbi_loadf
	(
		(Files::GetResourceDirectory() + path.data()).c_str(),
		&width,
		&height,
		&channels,
		STBI_rgb
	);

	if (data == nullptr)
	{
		LOG_ERROR("Unable to load texture: {}\n", path);
	}

	return data;
}

void Texture::CreateTexture()
{
	glGenTextures(1, &id);
}

void Texture::SetPixelParameter(GLenum name, GLint param)
{
	glPixelStorei(name, param);
}

void Texture::SetParameter(GLenum name, GLint param)
{
	glTexParameteri(type, name, param);
}

void Texture::SetParameter(GLenum name, GLfloat param)
{
	glTexParameterf(type, name, param);
}

void Texture::SetParameter(GLenum name, const GLfloat* param)
{
	glTexParameterfv(type, name, param);
}

void Texture::LoadImageData(u8* data, GLint internalFormat, GLint format, GLint dataType, GLenum target)
{
	glTexImage2D
	(
		target ? target : type,
		0,
		internalFormat,
		width,
		height,
		0,
		format,
		dataType,
		data
	);
}

void Texture::LoadImageData3D(u8* data, GLint internalFormat, GLint format, GLint dataType, GLenum target)
{
	glTexImage3D
	(
		target ? target : type,
		0,
		internalFormat,
		width,
		height,
		depth,
		0,
		format,
		dataType,
		data
	);
}

void Texture::GenerateMipmaps()
{
	glGenerateMipmap(type);
}

void Texture::Bind() const
{
	glBindTexture(type, id);
}

void Texture::Unbind() const
{
	glBindTexture(type, 0);
}

Texture::~Texture()
{
	// ID must not be zero
	if (id)
	{
		glDeleteTextures(1, &id);
	}
}