#include "ShadowBuffer.h"

using namespace Renderer;

using Detail::ShadowBufferGLSL;

using Mat4s = ShadowBuffer::Mat4s;

ShadowBuffer::ShadowBuffer()
	: UniformBuffer(4, sizeof(ShadowBufferGLSL))
{
}

void ShadowBuffer::LoadMatrices(const ShadowBuffer::Mat4s& matrices)
{
	auto size = std::min(matrices.size(), SHADOW_MAX_FRUSTUMS) * sizeof(glm::mat4);

	glBindBuffer(GL_UNIFORM_BUFFER, id);
	glBufferSubData
	(
		GL_UNIFORM_BUFFER,
		static_cast<GLintptr>(offsetof(ShadowBufferGLSL, matrices)),
		static_cast<GLsizeiptr>(size),
		reinterpret_cast<const void*>(&matrices[0])
	);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void ShadowBuffer::LoadDistances(const std::vector<f32>& distances)
{
	auto distancesGLSL = std::vector<GL::FloatGLSL>(distances.size());

	// Convert to glsl friendly data
	for (usize i = 0; i < distances.size(); ++i)
	{
		distancesGLSL[i] = {distances[i]};
	}

	glBindBuffer(GL_UNIFORM_BUFFER, id);

	glBufferSubData
	(
		GL_UNIFORM_BUFFER,
		static_cast<GLintptr>(offsetof(ShadowBufferGLSL, cascadeDistances)),
		static_cast<GLsizeiptr>(distancesGLSL.size() * sizeof(GL::FloatGLSL)),
		reinterpret_cast<const void*>(distancesGLSL.data())
	);

	// Convert to glsl friendly data
	GL::IntGLSL count = {static_cast<GLint>(distances.size())};

	glBufferSubData
	(
		GL_UNIFORM_BUFFER,
		static_cast<GLintptr>(offsetof(ShadowBufferGLSL, cascadeCount)),
		static_cast<GLsizeiptr>(sizeof(GL::IntGLSL)),
		reinterpret_cast<const void*>(&count)
	);

	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}
