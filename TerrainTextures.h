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
		using TX = std::shared_ptr<Renderer::Texture>;

		TerrainTextures(TX background, TX rTexture, TX gTexture, TX bTexture, TX blendMap)
			: background(background), rTexture(rTexture), gTexture(gTexture), bTexture(bTexture), blendMap(blendMap) {}

		TX background;
		TX rTexture;
		TX gTexture;
		TX bTexture;
		TX blendMap;
	};
}


#endif