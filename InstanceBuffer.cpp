#include "InstanceBuffer.h"

#include "Util.h"
#include "Maths.h"

using namespace Renderer;

using Detail::InstancedDataGLSL;
using Entities::Entity;

using DataVector = InstanceBuffer::DataVector;

InstanceBuffer::InstanceBuffer()
	: ShaderStorageBuffer(3, NUM_MAX_ENTITIES * sizeof(InstancedDataGLSL), GL_DYNAMIC_DRAW)
{
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