#include "SharedBuffer.h"

#include <GL/glew.h>

using namespace Renderer;

using Detail::SharedBufferGLSL;
using Entities::Camera;

SharedBuffer::SharedBuffer()
	: UniformBuffer(2, sizeof(SharedBufferGLSL), GL_STATIC_DRAW)
{
	// Bind buffer
	glBindBuffer(GL_UNIFORM_BUFFER, id);
	// Initialise empty data
	SharedBufferGLSL sharedBuffer = {};
	// Buffer empty data
	glBufferSubData
	(
		GL_UNIFORM_BUFFER,
		static_cast<GLintptr>(0),
		static_cast<GLsizeiptr>(sizeof(SharedBufferGLSL)),
		reinterpret_cast<const void*>(&sharedBuffer)
	);
	// Unbind buffer
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void SharedBuffer::LoadClipPlane(const glm::vec4& clipPlane)
{
	// Load value
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

void SharedBuffer::LoadCameraPos(const Camera& camera)
{
	// Adjust position
	auto position = glm::vec4(camera.position, 1.0f);
	// Load value
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
	// Pack values
	auto resolution = glm::vec4(dimensions, nearPlane, farPlane);
	// Buffer data
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