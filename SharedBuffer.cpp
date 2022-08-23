#include "SharedBuffer.h"

#include <GL/glew.h>

using namespace Renderer;

using Detail::SharedBufferGLSL;
using Entities::Camera;

SharedBuffer::SharedBuffer()
	: UniformBuffer(2, sizeof(SharedBufferGLSL))
{
}

void SharedBuffer::LoadClipPlane(const glm::vec4& clipPlane)
{
	glBindBuffer(GL_UNIFORM_BUFFER, id);
	glBufferSubData
	(
		GL_UNIFORM_BUFFER,
		static_cast<GLint>(offsetof(SharedBufferGLSL, clipPlane)),
		static_cast<GLsizeiptr>(sizeof(glm::vec4)),
		reinterpret_cast<const void*>(&clipPlane[0])
	);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void SharedBuffer::LoadSkyColor(const glm::vec4& skyColor)
{
	glBindBuffer(GL_UNIFORM_BUFFER, id);
	glBufferSubData
	(
		GL_UNIFORM_BUFFER,
		static_cast<GLint>(offsetof(SharedBufferGLSL, skyColor)),
		static_cast<GLsizeiptr>(sizeof(glm::vec4)),
		reinterpret_cast<const void*>(&skyColor[0])
	);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void SharedBuffer::LoadCameraPos(const Camera& camera)
{
	glBindBuffer(GL_UNIFORM_BUFFER, id);
	glm::vec4 position = glm::vec4(camera.position, 1.0f);
	glBufferSubData
	(
		GL_UNIFORM_BUFFER,
		static_cast<GLint>(offsetof(SharedBufferGLSL, cameraPos)),
		static_cast<GLsizeiptr>(sizeof(glm::vec4)),
		reinterpret_cast<const void*>(&position[0])
	);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void SharedBuffer::LoadFrustum(const glm::vec2& frustum)
{
	glBindBuffer(GL_UNIFORM_BUFFER, id);
	glBufferSubData
	(
		GL_UNIFORM_BUFFER,
		static_cast<GLint>(offsetof(SharedBufferGLSL, frustum)),
		static_cast<GLsizeiptr>(sizeof(glm::vec2)),
		reinterpret_cast<const void*>(&frustum[0])
	);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}
