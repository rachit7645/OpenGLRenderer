#include "InstanceBuffer.h"

#include "Util.h"
#include "Maths.h"

using namespace Renderer;

using Detail::InstancedDataGLSL;
using Detail::InstancedBufferGLSL;
using Entities::Entity;

using DataVector = InstanceBuffer::DataVector;

// Since we update this before every draw call, it's stored as GL_DYNAMIC_DRAW
InstanceBuffer::InstanceBuffer()
	: ShaderStorageBuffer(3, sizeof(InstancedBufferGLSL), GL_DYNAMIC_DRAW)
{
	// Bind buffer
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, id);
	// Initialise empty data
	InstancedBufferGLSL instancedBuffer = {};
	// Buffer empty data
	glBufferSubData
	(
		GL_SHADER_STORAGE_BUFFER,
		static_cast<GLintptr>(0),
		static_cast<GLsizeiptr>(sizeof(InstancedBufferGLSL)),
		reinterpret_cast<const void*>(&instancedBuffer)
	);
	// Unbind buffer
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

void InstanceBuffer::LoadInstanceData(const EntityVector& entities)
{
	auto data = GenerateData(entities);
	glBufferSubData
	(
		GL_SHADER_STORAGE_BUFFER,
		static_cast<GLintptr>(0),
		GetSize(entities, data),
		reinterpret_cast<const void*>(&data[0])
	);
}

GLsizeiptr InstanceBuffer::GetSize(const EntityVector& entities, const DataVector& data)
{
	// Choose the one with the length size
	auto length = std::min(data.size(), entities.size());
	// Calculate the size of the data
	auto size = length * sizeof(InstancedDataGLSL);
	// Return as GLsizeiptr
	return static_cast<GLsizeiptr>(size);
}

DataVector InstanceBuffer::GenerateData(const EntityVector& entities)
{
	auto data = DataVector(entities.size());

	for (usize i = 0; i < entities.size(); ++i)
	{
		// Load model matrix
		data[i].modelMatrix = Maths::CreateModelMatrix
		(
			entities[i]->position,
			entities[i]->rotation,
			entities[i]->scale
		);
	}

	return data;
}

void InstanceBuffer::Bind() const
{
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, id);
}

void InstanceBuffer::Unbind() const
{
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}