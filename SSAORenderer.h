#ifndef SSAO_RENDERER_H
#define SSAO_RENDERER_H

#include <memory>

#include "SSAOShader.h"
#include "SSAOBuffers.h"
#include "GBuffer.h"
#include "VertexArray.h"

namespace Renderer
{
	class SSAORenderer
	{
	public:
		using VAO = std::shared_ptr<VertexArray>;

		SSAORenderer
		(
			Shader::SSAOShader& shader,
			Renderer::SSAOBuffers& ssaoBuffers,
			Renderer::GBuffer& gBuffer
		);

		Shader::SSAOShader& shader;
		Renderer::SSAOBuffers& ssaoBuffers;
		Renderer::GBuffer& gBuffer;

		void RenderSSAO();
	private:
		VAO m_vao;
	};
}

#endif
