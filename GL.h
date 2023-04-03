#ifndef RACHIT_GL_H
#define RACHIT_GL_H

#include <string_view>
#include <GL/glew.h>

#include "GLM.h"

namespace GL
{
    // Initialize OpenGL info
    void Init(const glm::ivec2& dimensions);

	// Debug message callback
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

	// Get integer
	GLint GetIntegerv(GLenum param);
	// Get string
	std::string_view GetString(GLenum name);

	namespace Detail
	{
		// std140 data storage rules
		template<typename T>
		struct DataSTD140
		{
			// Stored data
			alignas(16) T data = {};
		};
	}

	// A float in std140 form
	using Float = Detail::DataSTD140<GLfloat>;
	// An int in std140 form
	using Int = Detail::DataSTD140<GLint>;

    // STD140 data alignment
    #define ALIGN_GLSL_STD140 alignas(16)
}

namespace GLEW
{
	// Get string
	std::string_view GetString(GLenum name);
	// Check is extension is supported
	bool GetExtension(const std::string_view name);
}

#endif