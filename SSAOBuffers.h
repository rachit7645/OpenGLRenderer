#ifndef SSAO_BUFFERS_H
#define SSAO_BUFFERS_H

#include <memory>

#include "FrameBuffer.h"
#include "SSAOBuffer.h"
#include "Texture.h"

namespace Renderer
{
	class SSAOBuffers
	{
	public:
		// Usings
		using FbPtr     = std::shared_ptr<FrameBuffer>;
		using TxPtr     = std::shared_ptr<Texture>;
		using BufferPtr = std::shared_ptr<SSAOBuffer>;

		// Main constructor
		SSAOBuffers();

		// Bind main SSAO buffer
		void BindSSAOBuffer() const;
		// Bind SSAO blur buffer
		void BindSSAOBlurBuffer() const;
		// Bind default FBO
		void BindDefaultFBO() const;

		// Framebuffers
		FbPtr ssaoBuffer;
		FbPtr ssaoBlurBuffer;

		// Noise texture
		TxPtr noiseTexture;
	private:
		// SSAO Buffer
		BufferPtr m_ssaoBuffer;

		// Initialises buffers
		void InitBuffers();
		// Load noise data
		void CreateNoiseTexture();
		// Load sample data
		void LoadKernels();
	};
}

#endif
