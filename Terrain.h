#ifndef TERRAIN_H
#define TERRAIN_H

#include <vector>
#include <string>
#include <limits>
#include <memory>

#include "GLM.h"
#include "Util.h"
#include "VertexArray.h"
#include "Material.h"
#include "TerrainTextures.h"
#include "Image.h"
#include "Entity.h"

namespace Terrains
{
	constexpr auto TERRAIN_SIZE          = 500.0f;
	constexpr auto TERRAIN_MAX_HEIGHT    = 50.0f;
	constexpr auto IMAGE_MAX_PIXEL_COLOR = std::numeric_limits<u8>::max();

	class Terrain
	{
	public:
		using VAO    = std::shared_ptr<Renderer::VertexArray>;
		using ImgPtr = std::shared_ptr<Util::Image2D>;

		Terrain
		(
			const std::string& hMapPath,
			const glm::vec2& position,
			const TerrainTextures& textures
		);

		glm::vec2 position;
		glm::vec2 gridPosition;

		TerrainTextures textures;
		VAO             vao;

		Util::Array2D<f32> heights;

		f32 GetHeight(const glm::vec2& worldPos)      const;
		f32 GetHeight(const Entities::Entity& entity) const;
	private:
		f32       CalculateHeight(int x, int z, const ImgPtr& hMap);
		glm::vec3 CalculateNormal(int x, int z, const ImgPtr& hMap);
	};

	const Terrain* GetCurrent(const std::vector<Terrain>& terrains, const glm::vec2& position);
	const Terrain* GetCurrent(const std::vector<Terrain>& terrains, const Entities::Entity& entity);
}

#endif // TERRAIN_H