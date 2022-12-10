#ifndef RACHIT_GL_H
#define RACHIT_GL_H

#include <string_view>
#include <GL/glew.h>

#include "GLM.h"

// TODO: Add opengl state checks

// Quick wrappers around OpenGL calls
namespace GL
{
	GLint            GetIntegerv(GLenum param);
	std::string_view GetString(GLenum name);

	// glDebugMessageCallback Function
	void CheckErrors
	(
		GLenum source,
		GLenum type,
		GLuint id,
		GLenum severity,
		GLsizei length,
		const GLchar* message,
		const void* userParam
	);

	// Initialize OpenGL info
	void Init(const glm::ivec2& dimensions);
	// Log debug information
	void LogDebugInfo();

	namespace Detail
	{
		template<typename T>
		struct DataSTD140
		{
			alignas(16) T data = {};
		};
	}

	using Float = Detail::DataSTD140<GLfloat>;
	using Int   = Detail::DataSTD140<GLint>;
}

#endif