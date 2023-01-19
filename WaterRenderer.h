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
		// Prepare renderer
		void Prepare();
		// Prepare each water
		void PrepareWater(const Waters::WaterTile& water);
		// Unbind renderer
		void Unbind();

		// VAO
		VAO m_vao;
		// Movement factor
		f32 m_moveFactor = 0.0f;
	};
}

#endif
