#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <memory>
#include <GL/glew.h>

#include "Texture.h"
#include "RenderBuffer.h"
#include "Util.h"

namespace Renderer
{
	enum class FBType
	{
		None,
		Color,
		Depth,
		ColorAndDepth,
		Empty
	};

	enum FBFilter : GLint
	{
		Linear  = GL_LINEAR,
		Nearest = GL_NEAREST
	};

	class FrameBuffer
	{
	public:
		using TxPtr    = std::shared_ptr<Texture>;
		using RdBufPtr = std::shared_ptr<RenderBuffer>;

		// Default constructor
		FrameBuffer() = default;
		// Main constructor
		FrameBuffer(GLsizei width, GLsizei height, FBType type, FBFilter filter = FBFilter::Linear);
		// Destructor
		~FrameBuffer();

		void Bind()   const;
		void Unbind() const;

		// Framebuffer dimensions
		GLsizei width  = 0;
		GLsizei height = 0;

		// Framebuffer type
		FBType   type   = FBType::None;
		FBFilter filter = FBFilter::Linear;
		// Framebuffer ID
		GLuint id = 0;

		// Textures
		TxPtr colorTexture;
		TxPtr depthTexture;

		// RenderBuffers
		RdBufPtr colorRenderBuffer;
		RdBufPtr depthRenderBuffer;
	private:
		void CreateColorTexture();
		void CreateDepthTexture();
		void CreateColorBuffer();
		void CreateDepthBuffer();
		void SetColorBuffer();
		void CheckStatus();
	};
}

#endif