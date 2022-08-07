#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>
#include <GL/glew.h>

namespace Renderer
{
	constexpr auto TEXTURE_LOD_BIAS = -0.5f;

	class Texture
	{
	public:
		// Loads a texture into memory, then an OpenGL object
		explicit Texture(const std::string& path);
		// Empty texture
		Texture(GLsizei width, GLsizei height);
		// Destructor 
		~Texture();

		GLuint id = 0;

		int width    = 0;
		int height   = 0;
		int channels = 0;
	};
}
#endif // TEXTURE_H