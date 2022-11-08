#include "LightsBuffer.h"

#include <GL/glew.h>

#include "Util.h"

using namespace Renderer;

using Entities::Light;
using Detail::LightsBufferGLSL;

LightsBuffer::LightsBuffer()
	: UniformBuffer(1, sizeof(LightsBufferGLSL), GL_STATIC_DRAW)
{
}

void LightsBuffer::LoadLights(const std::vector<Light>& lights)
{
	// Data
	usize   size      = std::min(lights.size(), SHADER_MAX_LIGHTS);
	GL::Int numLights = {static_cast<GLint>(size)};

	// Bind buffer
	glBindBuffer(GL_UNIFORM_BUFFER, id);

	// Load number of lights
	glBufferSubData
	(
		GL_UNIFORM_BUFFER,
		static_cast<GLintptr>(offsetof(LightsBufferGLSL, numLights)),
		static_cast<GLsizeiptr>(sizeof(GL::Int)),
		reinterpret_cast<const void*>(&numLights)
	);

	// Load lights
	for (usize i = 0; i < size; ++i)
	{
		glBufferSubData
		(
			GL_UNIFORM_BUFFER,
			static_cast<GLintptr>(offsetof(LightsBufferGLSL, lights) + sizeof(Light) * i),
			static_cast<GLsizeiptr>(sizeof(Light)),
			reinterpret_cast<const void*>(&lights[i])
		);
	}

	// Unbind buffer
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}