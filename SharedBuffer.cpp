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
	auto position = glm::vec4(camera.position, 1.0f);
	glBindBuffer(GL_UNIFORM_BUFFER, id);
	glBufferSubData
	(
		GL_UNIFORM_BUFFER,
		static_cast<GLint>(offsetof(SharedBufferGLSL, cameraPos)),
		static_cast<GLsizeiptr>(sizeof(glm::vec4)),
		reinterpret_cast<const void*>(&position[0])
	);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void SharedBuffer::LoadFarPlane(f32 farPlane)
{
	glBindBuffer(GL_UNIFORM_BUFFER, id);
	glBufferSubData
	(
		GL_UNIFORM_BUFFER,
		static_cast<GLint>(offsetof(SharedBufferGLSL, farPlane)),
		static_cast<GLsizeiptr>(sizeof(f32)),
		reinterpret_cast<const void*>(&farPlane)
	);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}