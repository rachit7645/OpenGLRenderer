#include "LightsBuffer.h"

#include <GL/glew.h>

#include "Util.h"

using namespace Renderer;

using Entities::Light;

// HACK: GLSL's alignment is fucking weird, so upload vec4's

LightsBuffer::LightsBuffer()
	: UniformBuffer(1, sizeof(Light) * SHADER_MAX_LIGHTS)
{
}

void LightsBuffer::LoadLights(const std::vector<Light>& lights)
{
	static const Light emptyLight;

	glBindBuffer(GL_UNIFORM_BUFFER, id);
	for (usize i = 0; i < SHADER_MAX_LIGHTS; ++i)
	{
		if (i < lights.size())
		{
			glBufferSubData
			(
				GL_UNIFORM_BUFFER,
				static_cast<GLintptr>(sizeof(Light) * i),
				static_cast<GLsizeiptr>(sizeof(Light)),
				reinterpret_cast<const void*>(&lights[i])
			);
		}
		else
		{
			glBufferSubData
			(
				GL_UNIFORM_BUFFER,
				static_cast<GLintptr>(sizeof(Light) * i),
				static_cast<GLsizeiptr>(sizeof(Light)),
				reinterpret_cast<const void*>(&emptyLight)
			);
		}
	}
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}