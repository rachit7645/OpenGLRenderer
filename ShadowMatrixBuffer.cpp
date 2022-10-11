#include "ShadowMatrixBuffer.h"

using namespace Renderer;

using Detail::ShadowMatrixBufferGLSL;

using Mat4s = ShadowMatrixBuffer::Mat4s;

ShadowMatrixBuffer::ShadowMatrixBuffer()
	: UniformBuffer(4, sizeof(ShadowMatrixBufferGLSL))
{
}

void ShadowMatrixBuffer::LoadMatrices(const ShadowMatrixBuffer::Mat4s& matrices)
{
	auto size = std::min(matrices.size(), SHADOW_MAX_FRUSTUMS) * sizeof(glm::mat4);

	glBindBuffer(GL_UNIFORM_BUFFER, id);
	glBufferSubData
	(
		GL_UNIFORM_BUFFER,
		static_cast<GLintptr>(offsetof(ShadowMatrixBufferGLSL, matrices)),
		static_cast<GLsizeiptr>(size),
		reinterpret_cast<const void*>(&matrices[0])
	);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void ShadowMatrixBuffer::LoadDistances(const std::array<f32, 4>& distances)
{
	auto size = static_cast<s32>(distances.size());

	glBindBuffer(GL_UNIFORM_BUFFER, id);

	glBufferSubData
	(
		GL_UNIFORM_BUFFER,
		static_cast<GLintptr>(offsetof(ShadowMatrixBufferGLSL, cascadeDistances)),
		static_cast<GLsizeiptr>(size),
		reinterpret_cast<const void*>(&distances[0])
	);

	glBufferSubData
	(
		GL_UNIFORM_BUFFER,
		static_cast<GLintptr>(offsetof(ShadowMatrixBufferGLSL, cascadeCount)),
		static_cast<GLsizeiptr>(sizeof(s32)),
		reinterpret_cast<const void*>(&size)
	);

	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}
