#include "ShadowBuffer.h"

using namespace Renderer;

using Detail::ShadowBufferGLSL;

using Mat4s = ShadowBuffer::Mat4s;

ShadowBuffer::ShadowBuffer()
	: UniformBuffer(4, sizeof(ShadowBufferGLSL), GL_STATIC_DRAW)
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
	auto distancesGL = std::vector<GL::Float>(distances.size());

	// Convert to glsl friendly data
	for (usize i = 0; i < distances.size(); ++i)
	{
		distancesGL[i] = {distances[i]};
	}

	glBindBuffer(GL_UNIFORM_BUFFER, id);

	glBufferSubData
	(
		GL_UNIFORM_BUFFER,
		static_cast<GLintptr>(offsetof(ShadowBufferGLSL, cascadeDistances)),
		static_cast<GLsizeiptr>(distancesGL.size() * sizeof(GL::Float)),
		reinterpret_cast<const void*>(distancesGL.data())
	);

	// Convert to glsl friendly data
	GL::Int count = {static_cast<GLint>(distances.size())};

	glBufferSubData
	(
		GL_UNIFORM_BUFFER,
		static_cast<GLintptr>(offsetof(ShadowBufferGLSL, cascadeCount)),
		static_cast<GLsizeiptr>(sizeof(GL::Int)),
		reinterpret_cast<const void*>(&count)
	);

	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}
