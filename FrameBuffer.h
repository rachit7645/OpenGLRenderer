#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <memory>
#include <optional>
#include <GL/glew.h>

#include "Texture.h"
#include "RenderBuffer.h"

namespace Renderer
{
	enum FBType : size_t
	{
		Color         = 0,
		Depth         = 1,
		ColorAndDepth = 2
	};

	class FrameBuffer
	{
	public:
		using TxPtr    = std::shared_ptr<Texture>;
		using RdBufPtr = std::shared_ptr<RenderBuffer>;

		// Default constructor
		FrameBuffer() = default;
		// Main constructor
		FrameBuffer(GLsizei width, GLsizei height, FBType type);
		// Destructor
		~FrameBuffer();

		void Bind()   const;
		void Unbind() const;

		// Framebuffer ID
		GLuint id = 0;
		// Textures
		std::optional<TxPtr> colorTexture;
		std::optional<TxPtr> depthTexture;
		// RenderBuffer
		std::optional<RdBufPtr> renderBuffer;
	private:
		void CreateColorTexture(GLsizei width, GLsizei height);
		void CreateDepthTexture(GLsizei width, GLsizei height);
		void CreateDepthBuffer(GLsizei width, GLsizei height);
	};
}

#endif