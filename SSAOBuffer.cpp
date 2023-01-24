#include "SSAOBuffer.h"

// Using namespaces
using namespace Renderer;

// Usings
using Detail::SSAOBufferGLSL;

SSAOBuffer::SSAOBuffer()
	: UniformBuffer(5, sizeof(SSAOBufferGLSL), GL_STATIC_DRAW)
{
	// Bind buffer
	glBindBuffer(GL_UNIFORM_BUFFER, id);
	// Initialise empty data
	SSAOBufferGLSL ssaoBuffer = {};
	// Buffer empty data
	glBufferSubData
	(
		GL_UNIFORM_BUFFER,
		static_cast<GLintptr>(0),
		static_cast<GLsizeiptr>(sizeof(SSAOBufferGLSL)),
		reinterpret_cast<const void*>(&ssaoBuffer)
	);
	// Unbind buffer
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void SSAOBuffer::LoadKernels(const std::vector<glm::vec4>& kernels)
{
	// Bind UBO
	glBindBuffer(GL_UNIFORM_BUFFER, id);

	// Store kernel size
	GL::Int kernelSize = {static_cast<GLint>(kernels.size())};

	// Load to UBO
	glBufferSubData
	(
		GL_UNIFORM_BUFFER,
		static_cast<GLint>(offsetof(SSAOBufferGLSL, kernelSize)),
		static_cast<GLsizeiptr>(sizeof(GL::Int)),
		reinterpret_cast<const void*>(&kernelSize)
	);

	// Load kernels to UBO
	glBufferSubData
	(
		GL_UNIFORM_BUFFER,
		static_cast<GLint>(offsetof(SSAOBufferGLSL, samples)),
		static_cast<GLsizeiptr>(sizeof(glm::vec4) * kernels.size()),
		reinterpret_cast<const void*>(kernels.data())
	);

	// Unbind UBO
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}