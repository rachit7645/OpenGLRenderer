#include "SharedBuffer.h"

#include <GL/glew.h>

using namespace Renderer;

using Detail::SharedBufferGLSL;
using Entities::Camera;

SharedBuffer::SharedBuffer()
	: UniformBuffer(2, sizeof(SharedBufferGLSL), GL_STATIC_DRAW)
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

void SharedBuffer::LoadResolution(const glm::ivec2& dimensions, f32 nearPlane, f32 farPlane)
{
	auto resolution = glm::vec4(dimensions, nearPlane, farPlane);
	glBindBuffer(GL_UNIFORM_BUFFER, id);
	glBufferSubData
	(
		GL_UNIFORM_BUFFER,
		static_cast<GLint>(offsetof(SharedBufferGLSL, resolution)),
		static_cast<GLsizeiptr>(sizeof(glm::vec4)),
		reinterpret_cast<const void*>(&resolution)
	);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}