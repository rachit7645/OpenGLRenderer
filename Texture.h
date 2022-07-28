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
		Texture(const std::string& path);
		// Destructor 
		~Texture();

		GLuint id;
		int width;
		int height;
		int channels;
	};
}
#endif // TEXTURE_H