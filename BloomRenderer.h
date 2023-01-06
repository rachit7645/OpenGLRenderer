#ifndef BLOOM_RENDERER_H
#define BLOOM_RENDERER_H

#include <memory>

#include "VertexArray.h"
#include "DownSampleShader.h"
#include "UpSampleShader.h"
#include "LightingBuffer.h"
#include "BloomBuffer.h"

namespace Renderer
{
	class BloomRenderer
	{
	public:
		// Usings
		using VAO = std::shared_ptr<VertexArray>;
		// Main constructor
		BloomRenderer
		(
			Shader::DownSampleShader& downShader,
			Shader::UpSampleShader& upShader,
			Renderer::LightingBuffer& lightingBuffer,
			Renderer::BloomBuffer& bloomBuffer
		);
		// Data
		Shader::DownSampleShader& downShader;
		Shader::UpSampleShader&   upShader;
		Renderer::LightingBuffer& lightingBuffer;
		Renderer::BloomBuffer&    bloomBuffer;
		// Render down samples
		void RenderDownSamples();
		// Render up samples
		void RenderUpSamples();
	private:
		// Quad VAO
		VAO m_vao;
	};
}

#endif
