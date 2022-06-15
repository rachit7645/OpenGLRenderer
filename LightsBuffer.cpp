#include "LightsBuffer.h"

using namespace Renderer;

using Entities::Light;

// HACK: GLSL's alignment is fucking weird, so upload vec4's

const Light emptyLight = Light(glm::vec3(0.0f), glm::vec3(0.0f));

LightsBuffer::LightsBuffer() : UniformBuffer(1, sizeof(Light)* SHADER_MAX_LIGHTS) {}

void LightsBuffer::LoadLight(const std::vector<Light>& lights)
{
	glBindBuffer(GL_UNIFORM_BUFFER, id);
	for (size_t i = 0; i < SHADER_MAX_LIGHTS; ++i)
	{
		if (i < lights.size())
		{
			glBufferSubData(GL_UNIFORM_BUFFER, sizeof(Light) * i, sizeof(Light), reinterpret_cast<const void*>(&lights[i]));
		}
		else
		{
			glBufferSubData(GL_UNIFORM_BUFFER, sizeof(Light) * i, sizeof(Light), reinterpret_cast<const void*>(&emptyLight));
		}
	}
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}