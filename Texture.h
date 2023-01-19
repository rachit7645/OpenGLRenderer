#ifndef TEXTURE_H
#define TEXTURE_H

#include <array>
#include <string_view>
#include <memory>
#include <GL/glew.h>

#include "Util.h"

namespace Renderer
{
	// Shared LOD bias
	constexpr f32 TEXTURE_LOD_BIAS = -0.5f;

	class Texture
	{
	public:
		// Usings
		using TxPtr = std::shared_ptr<Texture>;
		// Default constructor
		Texture() = default;
		// Loads a texture into memory, then an OpenGL object
		explicit Texture(const std::string_view path);
		// Loads a cube map texture
		explicit Texture(const std::array<const std::string_view, 6>& files);
		// Destructor 
		virtual ~Texture();

		void Bind(GLuint unit);

		// Opengl Data
		GLuint id   = 0;
		GLenum type = GL_TEXTURE_2D;

		// Internal image data
		int width    = 0;
		int height   = 0;
		int depth    = 0;
		int channels = 0;
	protected:
		// Creates the texture
		void CreateTexture();

		// Sets an integer pixel parameter
		void SetPixelParameter(GLenum name, GLint param);
		// Sets an integer texture parameter
		void SetParameter(GLenum name, GLint param);
		// Sets a float texture parameter
		void SetParameter(GLenum name, GLfloat param);
		// Sets an float array texture parameter
		void SetParameter(GLenum name, const GLfloat* param);

		// Sets 2D texture storage
		void Storage2D(GLenum internalFormat);
		// Set 3D texture storage
		void Storage3D(GLenum internalFormat);

		// Sets 2D texture storage with mipmaps
		void Storage2DMipMap(GLenum internalFormat);
		// Set 3D texture storage with mipmaps
		void Storage3DMipMap(GLenum internalFormat);

		// Loads image data
		void LoadImageData(const u8* data, GLint format, GLint dataType);
		// Loads 3D image data (ex: Cube Maps, Texture Arrays, etc.)
		void LoadImageData3D(const u8* data, GLint format, GLint dataType);

		// Generates mipmaps for the texture
		void GenerateMipmaps();

		// Loads a RGB8 image using stb image
		u8* LoadImage(const std::string_view path);
		// Loads a RGB16F image using stb image
		f32* LoadImageHDR(const std::string_view path);
	public:
		// All classes that can access the pipeline mode
		friend class FrameBuffer;
		friend class IBLMaps;
		friend class IBLRenderer;
		friend class BloomBuffer;
	};
}
#endif