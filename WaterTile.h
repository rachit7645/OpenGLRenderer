#ifndef WATER_TILE_H
#define WATER_TILE_H

#include <memory>

#include "GLM.h"
#include "Texture.h"

namespace Waters
{
	constexpr auto WATER_TILE_SIZE = 30;

	class WaterTile
	{
	public:
		using TxPtr = std::shared_ptr<Renderer::Texture>;
		// Default constructor
		WaterTile() = default;
		// Main constructor
		WaterTile(TxPtr dudvMap, TxPtr normalMap, const glm::vec3& position);

		// Maps
		TxPtr dudvMap;
		TxPtr normalMap;
		// Y: Height
		glm::vec3 position;
	};
}

#endif