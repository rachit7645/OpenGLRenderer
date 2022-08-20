#include "TerrainTextures.h"

using namespace Terrains;

TerrainTextures::TerrainTextures
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