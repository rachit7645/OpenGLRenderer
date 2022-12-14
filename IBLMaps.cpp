#include "IBLMaps.h"

#include "Settings.h"

// Using namespaces
using namespace Renderer;

// Usings
using Engine::Settings;

// Aliases
using TxPtr = IBLMaps::TxPtr;

IBLMaps::IBLMaps(IBLRenderer& iblRenderer)
{
	// Load HDR map
	auto hdrMap = LoadHDRMap();
	// Convert equiangular map to a cube map
	iblRenderer.ConvertToCubeMap(cubeMap, hdrMap);
	// Create the diffuse irradiance map
	iblRenderer.GenerateIrradiance(irradiance, cubeMap);
	// Create the pre=filtered specular map
	iblRenderer.PreFilterSpecular(preFilter, cubeMap);
	// Calculate BRDF lookup texture
	iblRenderer.CalculateBRDF(brdfLut);
}

TxPtr IBLMaps::LoadHDRMap()
{
	// Get settings
	const auto& settings = Settings::GetInstance();
	// Load HDR map
	TxPtr hdrMap = std::make_shared<Texture>();

	stbi_set_flip_vertically_on_load(true);
	auto data = hdrMap->LoadImageHDR(settings.ibl.envMapPath);
	stbi_set_flip_vertically_on_load(false);

	hdrMap->CreateTexture();
	hdrMap->Bind();

	hdrMap->SetParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	hdrMap->SetParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	hdrMap->SetParameter(GL_TEXTURE_WRAP_S,     GL_REPEAT);
	hdrMap->SetParameter(GL_TEXTURE_WRAP_T,     GL_REPEAT);
	hdrMap->SetParameter(GL_TEXTURE_LOD_BIAS,   TEXTURE_LOD_BIAS);

	hdrMap->SetPixelParameter(GL_UNPACK_ALIGNMENT, 1);
	hdrMap->LoadImageData(reinterpret_cast<u8*>(data), GL_RGB16F, GL_RGB, GL_FLOAT);

	hdrMap->Unbind();
	stbi_image_free(data);

	return hdrMap;
}