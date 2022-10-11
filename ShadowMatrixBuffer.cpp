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
		static_cast<GLintptr>(0),
		static_cast<GLsizeiptr>(size),
		reinterpret_cast<const void*>(&matrices[0])
	);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}