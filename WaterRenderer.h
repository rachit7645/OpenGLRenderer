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
		explicit WaterRenderer(Shader::WaterShader& shader, Waters::WaterFrameBuffers& waterFBOs);

		Shader::WaterShader& shader;
		Waters::WaterFrameBuffers& waterFBOs;

		void Render(const std::vector<Waters::WaterTile>& waters);
	private:
		void Prepare();
		void PrepareWater(const Waters::WaterTile& water);
		void Unbind();

		VAO m_vao;
		f32 m_moveFactor = 0.0f;
	};
}

#endif
