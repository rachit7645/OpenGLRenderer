#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <memory>
#include <GL/glew.h>

#include "Texture.h"
#include "RenderBuffer.h"
#include "Util.h"

namespace Waters
{
	class WaterFrameBuffers;
}

namespace Renderer
{
	class FrameBuffer
	{
	public:
		using TxPtr    = std::shared_ptr<Texture>;
		using RdBufPtr = std::shared_ptr<RenderBuffer>;

		// Default constructor
		FrameBuffer() = default;
		// Destructor
		~FrameBuffer();

		void Bind()   const;
		void Unbind() const;

		// Framebuffer dimensions
		GLsizei width  = 0;
		GLsizei height = 0;
		GLsizei depth  = 0;

		// Framebuffer ID
		GLuint id = 0;

		// Textures
		TxPtr colorTexture;
		TxPtr depthTexture;

		// RenderBuffers
		RdBufPtr colorRenderBuffer;
		RdBufPtr depthRenderBuffer;

	protected:
		void CreateFrameBuffer();

		void CreateColorTexture();
		void CreateColorBuffer();
		void SetColorBuffer(GLenum value);

		void CreateDepthTexture();
		void CreateDepthBuffer();
		void CreateDepthArrayTexture();

		void EnableDepth();

		void CheckStatus();
	public:
		friend class ShadowMap;
		friend class Waters::WaterFrameBuffers;
	};
}

#endif