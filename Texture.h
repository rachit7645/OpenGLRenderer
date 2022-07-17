#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>
#include <GL/glew.h>

namespace Renderer
{
	constexpr auto TEXTURE_LOD_BIAS = -0.5f;

	// TODO: Move to files
	// TODO: Convert to enum class
	enum PathType
	{
		RELATIVE = 0,
		ABSOLUTE = 1
	};

	class Texture
	{
	public:
		// Loads a texture into memory, then an OpenGL object
		Texture(const std::string& path, PathType pathType = PathType::RELATIVE);
		// Destructor 
		~Texture();

		GLuint id;
		int width;
		int height;
		int channels;
	};
}
#endif // TEXTURE_H