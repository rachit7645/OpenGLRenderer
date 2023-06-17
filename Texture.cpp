#include "Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

#include "Log.h"
#include "Files.h"
#include "Util.h"

// Using namespaces
using namespace Renderer;

// Usings
using Engine::Files;

Texture::Texture(const std::string_view path)
{
	// Create texture object
	CreateTexture();
	// Bind texture
	Bind();

	// Set parameters
	SetParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	SetParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	SetParameter(GL_TEXTURE_WRAP_S,     GL_REPEAT);
	SetParameter(GL_TEXTURE_WRAP_T,     GL_REPEAT);
	SetParameter(GL_TEXTURE_LOD_BIAS,   TEXTURE_LOD_BIAS);
	// Set pixel parameters
	SetPixelParameter(GL_UNPACK_ALIGNMENT, 1);

	// Load image
	auto data = LoadImage(path);
	// Load data
	LoadImageData(data, GL_RGB, GL_RGB, GL_UNSIGNED_BYTE);
	// Generate mipmaps
	GenerateMipmaps();

	// Unbind
	Unbind();
	// Free image data
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

	// For each file
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
	// Log
	LOG_INFO("Loading texture: {}\n", path);

	// Get files handle
	auto& files = Files::GetInstance();
	// Load image
	u8* data = stbi_load
	(
		(files.GetResources() + path.data()).c_str(),
		&width,
		&height,
		&channels,
		STBI_rgb
	);

	// If image load failed
	if (data == nullptr)
	{
		// Log error
		LOG_ERROR("Unable to load texture: {}\n", path);
	}

	// Return image data
	return data;
}

f32* Texture::LoadImageHDR(const std::string_view path)
{
	// Log
	LOG_INFO("Loading texture: {}\n", path);

	// Get files handle
	auto& files = Files::GetInstance();
	// Load image
	f32* data = stbi_loadf
	(
		(files.GetResources() + path.data()).c_str(),
		&width,
		&height,
		&channels,
		STBI_rgb
	);

	// If image load failed
	if (data == nullptr)
	{
		// Log error
		LOG_ERROR("Unable to load texture: {}\n", path);
	}

	// Return image data
	return data;
}

void Texture::CreateTexture()
{
	// Generate texture object
	glGenTextures(1, &id);
}

void Texture::SetPixelParameter(GLenum name, GLint param)
{
	// Set integer pixel storage parameter
	glPixelStorei(name, param);
}

void Texture::SetParameter(GLenum name, GLint param)
{
	// Set integer texture parameter
	glTexParameteri(type, name, param);
}

void Texture::SetParameter(GLenum name, GLfloat param)
{
	// Set float texture parameter
	glTexParameterf(type, name, param);
}

void Texture::SetParameter(GLenum name, const GLfloat* param)
{
	// Set float array texture parameter
	glTexParameterfv(type, name, param);
}

void Texture::LoadImageData(u8* data, GLint internalFormat, GLint format, GLint dataType, GLenum target)
{
	// Load image data
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
    LOG_DEBUG("Width: {}, Height: {}, Depth: {}\n", width, height, depth);

	// Load 3D image data
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

void Texture::GenerateMipmaps() const
{
	// Generate mipmap
	glGenerateMipmap(type);
}

void Texture::Bind() const
{
	// Bind texture
	glBindTexture(type, id);
}

void Texture::Unbind() const
{
	// Unbind texture
	glBindTexture(type, 0);
}

Texture::~Texture()
{
	// ID must not be zero
	if (id)
	{
		// Clear texture
		glDeleteTextures(1, &id);
	}
}