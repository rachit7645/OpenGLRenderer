#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <memory>
#include <array>
#include <GL/glew.h>

#include "Texture.h"
#include "RenderBuffer.h"
#include "Util.h"
#include "FBOAttachment.h"

// Forward declare to avoid circular includes
namespace Waters
{
	class WaterFrameBuffers;
}

namespace Renderer
{
	// This class is constructed in a pipeline-like form, thus providing absolute control
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

		// OpenGL spec guarantees that there will be at least 8 color attachments
		std::array<TxPtr, 8> colorTextures;
		// Only one depth texture is supported
		TxPtr depthTexture;

		// RenderBuffers
		RdBufPtr colorRenderBuffer;
		RdBufPtr depthRenderBuffer;
	protected:
		void CreateFrameBuffer();

		void AddTexture(TxPtr& texture, const FBOAttachment& attachment);
		void AddArrayTexture(TxPtr& texture, const FBOAttachment& attachment);
		void AddBuffer(RdBufPtr& buffer, const FBOAttachment& attachment);

		void SetDrawBuffer(GLenum value);
		void SetReadBuffer(GLenum value);
		void SetDrawBuffers(const std::vector<GLenum>& buffers);

		void EnableDepth();
		void CheckStatus();
	public:
		friend class ShadowMap;
		friend class GBuffer;
		friend class Waters::WaterFrameBuffers;
	};
}

#endif