#include "MatrixBuffer.h"

#include <GL/glew.h>

#include "Maths.h"

using namespace Renderer;

using Detail::MatrixBufferGLSL;
using Entities::Camera;

MatrixBuffer::MatrixBuffer()
	: UniformBuffer(0, sizeof(MatrixBufferGLSL), GL_STATIC_DRAW)
{
	// Bind buffer
	glBindBuffer(GL_UNIFORM_BUFFER, id);
	// Initialise empty data
	MatrixBufferGLSL matrixBuffer = {};
	// Buffer empty data
	glBufferSubData
	(
		GL_UNIFORM_BUFFER,
		static_cast<GLintptr>(0),
		static_cast<GLsizeiptr>(sizeof(MatrixBufferGLSL)),
		reinterpret_cast<const void*>(&matrixBuffer)
	);
	// Unbind buffer
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void MatrixBuffer::LoadView(const Camera& camera)
{
	// Create view matrix
	auto view = Maths::CreateViewMatrix(camera);
	// Load to UBO
	glBindBuffer(GL_UNIFORM_BUFFER, id);
	glBufferSubData
	(
		GL_UNIFORM_BUFFER,
		static_cast<GLint>(offsetof(MatrixBufferGLSL, viewMatrix)),
		static_cast<GLsizeiptr>(sizeof(glm::mat4)),
		reinterpret_cast<const void*>(&view[0][0])
	);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void MatrixBuffer::LoadProjection(const glm::mat4& projection)
{
	// Load to UBO
	glBindBuffer(GL_UNIFORM_BUFFER, id);
	glBufferSubData
	(
		GL_UNIFORM_BUFFER,
		static_cast<GLint>(offsetof(MatrixBufferGLSL, projectionMatrix)),
		static_cast<GLsizeiptr>(sizeof(glm::mat4)),
		reinterpret_cast<const void*>(&projection[0][0])
	);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}