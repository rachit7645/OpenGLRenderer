#ifndef TEXTURE_H
#define TEXTURE_H

#include <string_view>
#include <GL/glew.h>

#include "Util.h"

namespace Renderer
{
	constexpr auto TEXTURE_LOD_BIAS = -0.5f;

	class Texture
	{
	public:
		// Default constructor
		Texture() = default;
		// Loads a texture into memory, then an OpenGL object
		explicit Texture(const std::string_view path);
		// Destructor 
		~Texture();

		void Bind()   const;
		void Unbind() const;

		GLuint id   = 0;
		GLenum type = GL_TEXTURE_2D;

		int width    = 0;
		int height   = 0;
		int depth    = 0;
		int channels = 0;
	protected:
		void CreateTexture();

		void SetPixelParameter(GLenum name, GLint param);

		void SetParameter(GLenum name, GLint param);
		void SetParameter(GLenum name, GLfloat param);
		void SetParameter(GLenum name, const GLfloat* param);

		void LoadImageData(u8* data, GLint internalFormat, GLint format, GLint dataType);
		void LoadImageData3D(u8* data, GLint internalFormat, GLint format, GLint dataType);

		void GenerateMipmaps();
	private:
		u8* LoadImage(const std::string_view path);
	public:
		friend class FrameBuffer;
	};
}
#endif