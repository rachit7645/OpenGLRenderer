#ifndef RACHIT_GL_H
#define RACHIT_GL_H

#include <string_view>
#include <GL/glew.h>

#include "GLM.h"

namespace GL
{
	// Constants
	constexpr GLint DEFAULT_FRAME_BUFFER = 0;

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

	// Clears current frame color
	void ClearColor(GLuint id, const glm::vec4& color);
	// Clears current frame depth
	void ClearDepth(GLuint id, GLfloat depth);
	// Clears current frame depth and stencil
	void ClearDepthStencil(GLuint id, GLfloat depth, GLint stencil);

	// Initialize OpenGL info
	void Init(const glm::ivec2& dimensions);

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

namespace GLEW
{
	// Get string
	std::string_view GetString(GLenum name);
	// Check is extension is supported
	bool GetExtension(const std::string_view name);
}

#endif