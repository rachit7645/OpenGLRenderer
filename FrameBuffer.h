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
	class FrameBuffer
	{
	public:
		// Usings
		using TxPtr    = std::shared_ptr<Texture>;
		using RdBufPtr = std::shared_ptr<RenderBuffer>;

		// Default constructor
		FrameBuffer() = default;
		// Destructor
		~FrameBuffer();

		// Bind frame buffer
		void Bind() const;
		// Unbind frame buffer
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

		// Set buffers
		void SetDrawBuffer(GLenum value);
		void SetReadBuffer(GLenum value);
	protected:
		// Creates a new FBO
		void CreateFrameBuffer();

		// Add 2D texture
		void AddTexture(TxPtr& texture, const FBOAttachment& attachment);
		// Add cube map texture
		void AddTextureCubeMap(TxPtr& texture, const FBOAttachment& attachment);
		// Add texture array
		void AddTextureArray(TxPtr& texture, const FBOAttachment& attachment);
		// Add render buffer
		void AddBuffer(RdBufPtr& buffer, const FBOAttachment& attachment);

		// Set draw buffers
		void SetDrawBuffers(const std::vector<GLenum>& buffers);
		// Enable depth
		void EnableDepth();
		// Check status
		void CheckStatus();
	public:
		// Classes that can access pipeline mode
		friend class ShadowMap;
		friend class GBuffer;
		friend class IBLRenderer;
		friend class LightingBuffer;
		friend class BloomBuffer;
		friend class Waters::WaterFrameBuffers;
	};
}

#endif