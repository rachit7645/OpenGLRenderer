#ifndef SSAO_RENDERER_H
#define SSAO_RENDERER_H

#include <memory>

#include "SSAOShader.h"
#include "SSAOBlurShader.h"
#include "SSAOBuffers.h"
#include "GBuffer.h"
#include "VertexArray.h"

namespace Renderer
{
	class SSAORenderer
	{
	public:
		// Usings
		using VAO = std::shared_ptr<VertexArray>;

		// Main constructor
		SSAORenderer
		(
			Shader::SSAOShader& shader,
			Shader::SSAOBlurShader& blurShader,
			Renderer::SSAOBuffers& buffers,
			Renderer::GBuffer& gBuffer
		);

		// Shaders
		Shader::SSAOShader&     shader;
		Shader::SSAOBlurShader& blurShader;

		// Buffers
		Renderer::SSAOBuffers& buffers;
		Renderer::GBuffer&     gBuffer;

		// Render SSAO pass
		void RenderSSAO();
		// Render SSAO blur pass
		void RenderSSAOBlur();
	private:
		// Quad VAO
		VAO m_vao;
	};
}

#endif
