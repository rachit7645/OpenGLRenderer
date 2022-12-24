#ifndef RACHIT_GL_H
#define RACHIT_GL_H

#include <string_view>
#include <GL/glew.h>

#include "GLM.h"

// TODO: Add opengl state checks

// Quick wrappers around OpenGL calls
namespace GL
{
	// Get integer
	GLint GetIntegerv(GLenum param);
	// Get string
	std::string_view GetString(GLenum name);

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

	// Initialize OpenGL info
	void Init(const glm::ivec2& dimensions);
	// Log debug information
	void LogDebugInfo();

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
}

#endif