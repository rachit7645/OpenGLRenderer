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
			: background(background),
			  rTexture(rTexture), 
			  gTexture(gTexture), 
			  bTexture(bTexture), 
			  blendMap(blendMap) 
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