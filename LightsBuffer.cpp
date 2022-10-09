#include "LightsBuffer.h"

#include <GL/glew.h>

#include "Util.h"

using namespace Renderer;

using Entities::Light;
using Detail::LightsBufferGLSL;

LightsBuffer::LightsBuffer()
	: UniformBuffer(1, sizeof(LightsBufferGLSL))
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

void LightsBuffer::LoadLightProjection(const glm::mat4& projection)
{
	glBindBuffer(GL_UNIFORM_BUFFER, id);
	glBufferSubData
	(
		GL_UNIFORM_BUFFER,
		static_cast<GLint>(offsetof(LightsBufferGLSL, lightProj)),
		static_cast<GLsizeiptr>(sizeof(glm::mat4)),
		reinterpret_cast<const void*>(&projection[0][0])
	);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void LightsBuffer::LoadLightView(const glm::mat4& view)
{
	glBindBuffer(GL_UNIFORM_BUFFER, id);
	glBufferSubData
	(
		GL_UNIFORM_BUFFER,
		static_cast<GLint>(offsetof(LightsBufferGLSL, lightView)),
		static_cast<GLsizeiptr>(sizeof(glm::mat4)),
		reinterpret_cast<const void*>(&view[0][0])
	);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}