#ifndef TERRAIN_TEXTURES_H
#define TERRAIN_TEXTURES_H

#include <memory>

#include "Texture.h"

namespace Terrains
{
	class TerrainTextures
	{
	public:
		using TxPtr = std::shared_ptr<Renderer::Texture>;
		// Default constructor
		TerrainTextures() = default;
		// Main constructor
		TerrainTextures
		(
			TxPtr background,
			TxPtr rTexture,
			TxPtr gTexture,
			TxPtr bTexture,
			TxPtr blendMap
		);

		TxPtr background;
		TxPtr rTexture;
		TxPtr gTexture;
		TxPtr bTexture;
		TxPtr blendMap;
	};
}


#endif