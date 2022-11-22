#include "LightsBuffer.h"

#include <GL/glew.h>

#include "Util.h"

using namespace Renderer;

using Entities::PointLight;
using Entities::DirectionalLight;
using Detail::LightsBufferGLSL;

LightsBuffer::LightsBuffer()
	: UniformBuffer(1, sizeof(LightsBufferGLSL), GL_STATIC_DRAW)
{
}

void LightsBuffer::LoadDirectionalLights(const std::vector<DirectionalLight>& lights)
{
	// Data
	usize   size           = std::min(lights.size(), SHADER_MAX_LIGHTS);
	GL::Int numDirLights   = {static_cast<GLint>(size)};

	// Bind buffer
	glBindBuffer(GL_UNIFORM_BUFFER, id);

	// Load number of lights
	glBufferSubData
	(
		GL_UNIFORM_BUFFER,
		static_cast<GLintptr>(offsetof(LightsBufferGLSL, numDirLights)),
		static_cast<GLsizeiptr>(sizeof(GL::Int)),
		reinterpret_cast<const void*>(&numDirLights)
	);

	// Load lights
	for (usize i = 0; i < size; ++i)
	{
		glBufferSubData
		(
			GL_UNIFORM_BUFFER,
			static_cast<GLintptr>(offsetof(LightsBufferGLSL, dirLights) + sizeof(DirectionalLight) * i),
			static_cast<GLsizeiptr>(sizeof(DirectionalLight)),
			reinterpret_cast<const void*>(&lights[i])
		);
	}

	// Unbind buffer
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void LightsBuffer::LoadPointLights(const std::vector<PointLight>& lights)
{
	// Data
	usize   size           = std::min(lights.size(), SHADER_MAX_LIGHTS);
	GL::Int numPointLights = {static_cast<GLint>(size)};

	// Bind buffer
	glBindBuffer(GL_UNIFORM_BUFFER, id);

	// Load number of lights
	glBufferSubData
	(
		GL_UNIFORM_BUFFER,
		static_cast<GLintptr>(offsetof(LightsBufferGLSL, numPointLights)),
		static_cast<GLsizeiptr>(sizeof(GL::Int)),
		reinterpret_cast<const void*>(&numPointLights)
	);

	// Load lights
	for (usize i = 0; i < size; ++i)
	{
		glBufferSubData
		(
			GL_UNIFORM_BUFFER,
			static_cast<GLintptr>(offsetof(LightsBufferGLSL, pointLights) + sizeof(PointLight) * i),
			static_cast<GLsizeiptr>(sizeof(PointLight)),
			reinterpret_cast<const void*>(&lights[i])
		);
	}

	// Unbind buffer
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}