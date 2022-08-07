#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <memory>
#include <GL/glew.h>

#include "Texture.h"
#include "RenderBuffer.h"

namespace Renderer
{
	class FrameBuffer
	{
	public:
		using TxPtr    = std::shared_ptr<Texture>;
		using RdBufPtr = std::shared_ptr<RenderBuffer>;

		// Default constructor
		FrameBuffer() = default;
		// Main constructor
		FrameBuffer(GLsizei width, GLsizei height);
		// Destructor
		~FrameBuffer();

		void Bind()   const;
		void Unbind() const;

		// Framebuffer ID
		GLuint id = 0;
		// Texture
		TxPtr texture;
		// RenderBuffer
		RdBufPtr renderBuffer;
	};
}

#endif