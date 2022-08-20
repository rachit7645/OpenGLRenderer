#include "SharedBuffer.h"

#include <GL/glew.h>

using namespace Renderer;

using Detail::SharedBufferGLSL;

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