#include "MatrixBuffer.h"

#include <GL/glew.h>

#include "Maths.h"

// Using namespaces
using namespace Renderer;

// Usings
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
	// Bind buffer
	glBindBuffer(GL_UNIFORM_BUFFER, id);

	// Create view matrix
	auto view = Maths::CreateViewMatrix(camera);
	// Load view matrix to UBO
	glBufferSubData
	(
		GL_UNIFORM_BUFFER,
		static_cast<GLint>(offsetof(MatrixBufferGLSL, viewMatrix)),
		static_cast<GLsizeiptr>(sizeof(glm::mat4)),
		reinterpret_cast<const void*>(&view[0][0])
	);

	// Create inverse view matrix
	auto invView = glm::inverse(view);
	// Load inverse view matrix to UBO
	glBufferSubData
	(
		GL_UNIFORM_BUFFER,
		static_cast<GLint>(offsetof(MatrixBufferGLSL, invCameraView)),
		static_cast<GLsizeiptr>(sizeof(glm::mat4)),
		reinterpret_cast<const void*>(&invView[0][0])
	);

	// Unbind buffer
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void MatrixBuffer::LoadProjection(const glm::mat4& projection)
{
	// Bind buffer
	glBindBuffer(GL_UNIFORM_BUFFER, id);

	// Load projection matrix to UBO
	glBufferSubData
	(
		GL_UNIFORM_BUFFER,
		static_cast<GLint>(offsetof(MatrixBufferGLSL, projectionMatrix)),
		static_cast<GLsizeiptr>(sizeof(glm::mat4)),
		reinterpret_cast<const void*>(&projection[0][0])
	);

	// Create projection view matrix
	auto invProj = glm::inverse(projection);
	// Load inverse view matrix to UBO
	glBufferSubData
	(
		GL_UNIFORM_BUFFER,
		static_cast<GLint>(offsetof(MatrixBufferGLSL, invProjection)),
		static_cast<GLsizeiptr>(sizeof(glm::mat4)),
		reinterpret_cast<const void*>(&invProj[0][0])
	);

	// Unbind buffer
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}