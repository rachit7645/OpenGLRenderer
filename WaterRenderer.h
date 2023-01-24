#ifndef WATER_RENDERER_H
#define WATER_RENDERER_H

#include <memory>
#include <vector>

#include "WaterShader.h"
#include "VertexArray.h"
#include "WaterTile.h"
#include "WaterFrameBuffers.h"
#include "Texture.h"

namespace Renderer
{
	class WaterRenderer
	{
	public:
		// Usings
		using VAO = std::shared_ptr<VertexArray>;

		// Main constructor
		WaterRenderer(Shader::WaterShader& shader, Waters::WaterFrameBuffers& waterFBOs);

		// Data
		Shader::WaterShader& shader;
		Waters::WaterFrameBuffers& waterFBOs;

		// Render waters
		void Render(const std::vector<Waters::WaterTile>& waters);
	private:
		// Prepare render
		void Prepare();
		// Prepare water
		void PrepareWater(const Waters::WaterTile& water);
		// Unbind render
		void Unbind();

		// Tile VAO
		VAO m_vao;
		// Movement factor
		f32 m_moveFactor = 0.0f;
	};
}

#endif
