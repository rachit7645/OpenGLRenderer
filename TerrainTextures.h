#ifndef TERRAIN_TEXTURES_H
#define TERRAIN_TEXTURES_H

#include <memory>

#include "Util.h"
#include "Texture.h"

namespace Terrains
{
	class TerrainTextures
	{
	public:
		using TxPtr = std::shared_ptr<Renderer::Texture>;
		TerrainTextures
		(
			TxPtr background,
			TxPtr rTexture,
			TxPtr gTexture,
			TxPtr bTexture,
			TxPtr blendMap
		)
			: background(std::move(background)),
			  rTexture(std::move(rTexture)),
			  gTexture(std::move(gTexture)),
			  bTexture(std::move(bTexture)),
			  blendMap(std::move(blendMap))
		{
		}

		TxPtr background;
		TxPtr rTexture;
		TxPtr gTexture;
		TxPtr bTexture;
		TxPtr blendMap;
	};
}


#endif