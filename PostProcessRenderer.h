#ifndef POST_PROCESS_RENDERER_H
#define POST_PROCESS_RENDERER_H

#include <memory>

#include "VertexArray.h"
#include "LightingBuffer.h"
#include "PostProcessShader.h"

namespace Renderer
{
	class PostProcessRenderer
	{
	public:
		// Usings
		using VAO = std::shared_ptr<VertexArray>;
		// Main constructor
		PostProcessRenderer(Shader::PostProcessShader& shader, Renderer::LightingBuffer& lightingBuffer);
		// Data
		Shader::PostProcessShader& shader;
		Renderer::LightingBuffer&  lightingBuffer;
		// Render post process pass
		void Render();
	private:
		// Quad VAO
		VAO m_vao;
	};
}

#endif
