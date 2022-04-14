#ifndef TERRAIN_RENDERER_H
#define TERRAIN_RENDERER_H

#include <vector>

#include <gl/glew.h>
#include <glm/glm.hpp>

#include "Util.h"
#include "Terrain.h"
#include "TerrainShader.h"

namespace Renderer
{
	class TerrainRenderer
	{
	public:
		TerrainRenderer(Shader::TerrainShader& sh);
		Shader::TerrainShader& shader;

		void Render(std::vector<Terrains::Terrain>& terrains);
		void PrepareTerrain(Terrains::Terrain& terrain);
		void LoadModelMatrix(Terrains::Terrain& terrain);
		void UnbindTerrain();
	};
}

#endif