#ifndef TERRAIN_H
#define TERRAIN_H

#include <memory>
#include <vector>
#include <string>
#include <algorithm>
#include <limits>

#include "GLM.h"
#include "Util.h"
#include "VertexArray.h"
#include "Material.h"
#include "TerrainTextures.h"
#include "Image.h"
#include "Entity.h"

namespace Terrains
{
	constexpr auto TERRAIN_SIZE 		 = 500.0f;
	constexpr auto TERRAIN_MAX_HEIGHT 	 = 30.0f;
	constexpr auto IMAGE_MAX_PIXEL_COLOR = std::numeric_limits<u8>::max();

	class Terrain
	{
	public:
		Terrain(const std::string& hMapPath, const glm::vec2& position, const TerrainTextures& textures,
			const Renderer::Material& material = Renderer::Material());

		glm::vec2 position;
		glm::vec2 gridPosition;
		Util::Array2D<f32> heights;

		TerrainTextures textures;
		Renderer::Material material;
		std::shared_ptr<Renderer::VertexArray> vao;

		f32 GetHeight(const glm::vec2& worldPos) const;
		f32 GetHeight(const Entities::Entity& entity) const;
	private:
		f32 CalculateHeight(int x, int z, const Util::Image2D& hMap);
		glm::vec3 CalculateNormal(int x, int z, const Util::Image2D& hMap);
	};

	const Terrain* GetCurrent(const std::vector<Terrain>& terrains, const glm::vec2& position);
	const Terrain* GetCurrent(const std::vector<Terrain>& terrains, const Entities::Entity& entity);
}

#endif // TERRAIN_H