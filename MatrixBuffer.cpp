#include "MatrixBuffer.h"

#include <GL/glew.h>

#include "Maths.h"

using namespace Renderer;

using Detail::MatrixBufferGLSL;
using Entities::Camera;
using Entities::PointLight;

MatrixBuffer::MatrixBuffer()
	: UniformBuffer(0, sizeof(MatrixBufferGLSL), GL_STATIC_DRAW)
{
}

void MatrixBuffer::LoadView(const Camera& camera)
{
	auto view = Maths::CreateViewMatrix(camera);
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