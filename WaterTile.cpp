#include "WaterTile.h"

// Using namespaces
using namespace Waters;

// Usings
using Entities::Transform;

WaterTile::WaterTile(TxPtr dudvMap, TxPtr normalMap, const Entities::Transform& transform)
	: dudvMap(std::move(dudvMap)),
	  normalMap(std::move(normalMap)),
      transform(transform)
{
}