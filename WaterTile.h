#ifndef WATER_TILE_H
#define WATER_TILE_H

#include <memory>

#include "GLM.h"
#include "Texture.h"

namespace Waters
{
	// Tile size
	constexpr auto WATER_TILE_SIZE = 30;

	class WaterTile
	{
	public:
		// Usings
		using TxPtr = std::shared_ptr<Renderer::Texture>;

		// Default constructor
		WaterTile() = default;
		// Main constructor
		WaterTile(TxPtr dudvMap, TxPtr normalMap, const glm::vec3& position);

		// Maps
		TxPtr dudvMap;
		TxPtr normalMap;

		// Position
		glm::vec3 position = {0.0f, 0.0f, 0.0f};
	};
}

#endif