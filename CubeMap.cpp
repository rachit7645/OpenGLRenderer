#include "CubeMap.h"

#include "Util.h"
#include "Log.h"

using namespace Renderer;

CubeMap::CubeMap(const std::array<const std::string_view, 6>& files)
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

CubeMap::CubeMap()
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

	LoadImageData3D
	(
		nullptr,
		GL_RGB16F,
		GL_RGB,
		GL_FLOAT
	);

	// Unbind
	Unbind();
}