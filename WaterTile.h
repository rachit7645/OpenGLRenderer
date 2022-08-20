#ifndef WATER_TILE_H
#define WATER_TILE_H

#include "GLM.h"

namespace Waters
{
	constexpr auto WATER_TILE_SIZE = 30;

	class WaterTile
	{
	public:
		// Default constructor
		WaterTile() = default;
		// Main constructor
		explicit WaterTile(const glm::vec3& position);

		// X: X Pos
		// Y: Height
		// Z: Z Pos
		glm::vec3 position;
	};
}

#endif
