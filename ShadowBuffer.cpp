#include "ShadowBuffer.h"

// Using namespaces
using namespace Renderer;

// Aliases
using ShadowBufferGLSL = ShadowBuffer::ShadowBufferGLSL;
using Mat4s            = ShadowBuffer::Mat4s;

ShadowBuffer::ShadowBuffer()
	: UniformBuffer(4, sizeof(ShadowBufferGLSL), GL_STATIC_DRAW)
{
	// Bind buffer
	glBindBuffer(GL_UNIFORM_BUFFER, id);
	// Initialise empty data
	ShadowBufferGLSL shadowBuffer = {};
	// Buffer empty data
	glBufferSubData
	(
		GL_UNIFORM_BUFFER,
		static_cast<GLintptr>(0),
		static_cast<GLsizeiptr>(sizeof(ShadowBufferGLSL)),
		reinterpret_cast<const void*>(&shadowBuffer)
	);
	// Unbind buffer
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void ShadowBuffer::LoadMatrices(const ShadowBuffer::Mat4s& matrices)
{
	// Calculate size
	auto size = std::min(matrices.size(), SHADOW_MAX_FRUSTUMS) * sizeof(glm::mat4);
	// Load to UBO
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
	// Bind
	glBindBuffer(GL_UNIFORM_BUFFER, id);

	// Initialise vector
	auto distancesGL = std::vector<GL::Float>(distances.size());
	// Convert to glsl friendly data
	for (usize i = 0; i < distances.size(); ++i)
	{
		distancesGL[i] = {distances[i]};
	}

	// Load distances
	glBufferSubData
	(
		GL_UNIFORM_BUFFER,
		static_cast<GLintptr>(offsetof(ShadowBufferGLSL, cascadeDistances)),
		static_cast<GLsizeiptr>(distancesGL.size() * sizeof(GL::Float)),
		reinterpret_cast<const void*>(distancesGL.data())
	);

	// Convert to glsl friendly data
	GL::Int count = {static_cast<GLint>(distances.size())};

	// Load count
	glBufferSubData
	(
		GL_UNIFORM_BUFFER,
		static_cast<GLintptr>(offsetof(ShadowBufferGLSL, cascadeCount)),
		static_cast<GLsizeiptr>(sizeof(GL::Int)),
		reinterpret_cast<const void*>(&count)
	);

	// Unbind
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}
