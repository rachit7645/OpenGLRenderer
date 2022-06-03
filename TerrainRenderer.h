#ifndef TERRAIN_RENDERER_H
#define TERRAIN_RENDERER_H

#include <vector>
#include <gl/glew.h>

#include "GLM.h"
#include "Util.h"
#include "Terrain.h"
#include "TerrainShader.h"

namespace Renderer
{
	class TerrainRenderer
	{
	public:
		TerrainRenderer(Shader::TerrainShader& shaderRef);
		Shader::TerrainShader& shader;

		void Render(const std::vector<Terrains::Terrain>& terrains);
	private:
		void PrepareTerrain(const Terrains::Terrain& terrain);
		void BindTextures(const Terrains::Terrain& terrain);
		void LoadModelMatrix(const Terrains::Terrain& terrain);
		void UnbindTerrain();
	};
}

#endif