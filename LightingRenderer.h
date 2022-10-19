#ifndef LIGHTING_RENDERER_H
#define LIGHTING_RENDERER_H

#include <memory>

#include "LightingShader.h"
#include "VertexArray.h"
#include "GBuffer.h"
#include "ShadowMap.h"

namespace Renderer
{
	class LightingRenderer
	{
	public:
		// Usings
		using VAO = std::shared_ptr<VertexArray>;
		// Main constructor
		LightingRenderer
		(
			Shader::LightingShader& shader,
			Renderer::ShadowMap& shadowMap,
			Renderer::GBuffer& gBuffer
		);
		// Data
		Shader::LightingShader& shader;
		Renderer::ShadowMap&    shadowMap;
		Renderer::GBuffer&      gBuffer;

		// Render light pass
		void Render();
	private:
		VAO m_vao;
	};
}

#endif
