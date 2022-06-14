#include "LightsBuffer.h"

using namespace Renderer;
using Entities::Light;

// HACK: GLSL's alignment is fucking weird, so upload vec4's

LightsBuffer::LightsBuffer() : UniformBuffer(1, sizeof(glm::vec4) * 2) {}

void LightsBuffer::LoadLight(const Entities::Light& light)
{
	// Convert to vec4
	glm::vec4 lightPos = { light.position, 1.0f };
	glm::vec4 lightCol = { light.colour, 1.0f };

	glBindBuffer(GL_UNIFORM_BUFFER, id);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::vec4), &lightPos);
	glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::vec4), sizeof(glm::vec4), &lightCol);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}