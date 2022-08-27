#include "WaterTile.h"

using namespace Waters;

WaterTile::WaterTile(TxPtr dudvMap, TxPtr normalMap, const glm::vec3& position)
	: dudvMap(std::move(dudvMap)),
	  normalMap(std::move(normalMap)),
	  position(position)
{
}