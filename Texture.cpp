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
	// Check
	assert(id != 0 && width > 0 && height > 0 && type == GL_TEXTURE_2D);
	// Allocate memory
	Storage2DMipMap(GL_RGB8);
	// Load data
	LoadImageData(data, GL_RGB, GL_UNSIGNED_BYTE);
	// Generate mipmaps
	GenerateMipmaps();

	// Free image data
	stbi_image_free(data);
}

Texture::Texture(const std::array<const std::string_view, 6>& files)
{
	// Set internal data
	type = GL_TEXTURE_CUBE_MAP;

	// Create texture
	CreateTexture();

	// Set parameters
	SetParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	SetParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	SetParameter(GL_TEXTURE_WRAP_S,     GL_CLAMP_TO_EDGE);
	SetParameter(GL_TEXTURE_WRAP_T,     GL_CLAMP_TO_EDGE);
	SetParameter(GL_TEXTURE_WRAP_R,     GL_CLAMP_TO_EDGE);
	SetParameter(GL_TEXTURE_LOD_BIAS,   TEXTURE_LOD_BIAS);

	// Set pixel parameters
	SetPixelParameter(GL_UNPACK_ALIGNMENT, 1);

	// Allocate memory
	Storage2D(GL_RGB16F);

	// For each file
	for (auto file : files)
	{
		// Load data
		f32* data = LoadImageHDR(file);
		// Upload to VRAM
		LoadImageData3D
		(
			reinterpret_cast<u8*>(data),
			GL_RGB,
			GL_FLOAT
		);
		// Free memory
		stbi_image_free(data);
	}
}

void Texture::Bind(GLuint unit)
{
	// Bind texture
	glBindTextureUnit(unit, id);
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
	glCreateTextures(type, 1, &id);
}

void Texture::SetPixelParameter(GLenum name, GLint param)
{
	// Set integer pixel storage parameter
	glPixelStorei(name, param);
}

void Texture::SetParameter(GLenum name, GLint param)
{
	// Set integer texture parameter
	glTextureParameteri(id, name, param);
}

void Texture::SetParameter(GLenum name, GLfloat param)
{
	// Set float texture parameter
	glTextureParameterf(id, name, param);
}

void Texture::SetParameter(GLenum name, const GLfloat* param)
{
	// Set float array texture parameter
	glTextureParameterfv(id, name, param);
}

void Texture::Storage2D(GLenum internalFormat)
{
	// Allocate storage for texture
	glTextureStorage2D
	(
		id,             // Texture
		1,              // Levels
		internalFormat, // Internal Format
		width,          // Width
		height          // Height
	);
}

void Texture::Storage3D(GLenum internalFormat)
{
	// Allocate storage for texture
	glTextureStorage3D
	(
		id,             // Texture
		1,              // Levels
		internalFormat, // Internal Format
		width,          // Width
		height,         // Height
		depth           // Depth
	);
}

void Texture::Storage2DMipMap(GLenum internalFormat)
{
	// Get the bigger dimension
	auto max = std::max(width, height);
	// Take the base 2 logarithm
	auto log2Max = std::log2(max);
	// Floor result and add 1 to get levels
	auto levels = 1 + static_cast<GLsizei>(std::floor(log2Max));

	// Allocate storage for texture
	glTextureStorage2D
	(
		id,             // Texture
		levels,         // Levels
		internalFormat, // Internal Format
		width,          // Width
		height          // Height
	);
}

void Texture::Storage3DMipMap(GLenum internalFormat)
{
	// Get the bigger dimension
	auto max = std::max(width, height);
	// Take the base 2 logarithm
	auto log2Max = std::log2(max);
	// Floor result and add 1 to get levels
	auto levels = 1 + static_cast<GLsizei>(std::floor(log2Max));

	// Allocate storage for texture
	glTextureStorage3D
	(
		id,             // Texture
		levels,         // Levels
		internalFormat, // Internal Format
		width,          // Width
		height,         // Height
		depth           // Depth
	);
}

void Texture::LoadImageData(const u8* data, GLint format, GLint dataType)
{
	// Load image data
	glTextureSubImage2D
	(
		id,       // Texture
		0,        // Level
		0,        // X-Offset
		0,        // Y-Offset
		width,    // Width
		height,   // Height
		format,   // Format
		dataType, // Type
		data      // Data
	);
}

void Texture::LoadImageData3D(const u8* data, GLint format, GLint dataType)
{
	// Load image data
	glTextureSubImage3D
	(
		id,       // Texture
		0,        // Level
		0,        // X-Offset
		0,        // Y-Offset
		0,        // Z-Offset
		width,    // Width
		height,   // Height
		depth,    // Depth
		format,   // Format
		dataType, // Type
		data      // Data
	);
}

void Texture::GenerateMipmaps()
{
	// Generate mipmap
	glGenerateTextureMipmap(id);
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