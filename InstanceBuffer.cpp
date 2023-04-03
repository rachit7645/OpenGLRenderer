#include "InstanceBuffer.h"

#include "Util.h"

// Using namespaces
using namespace Renderer;

// Usings
using Entities::Entity;

// Aliases
using InstanceDataGLSL   = InstanceBuffer::InstanceDataGLSL;
using InstanceBufferGLSL = InstanceBuffer::InstanceBufferGLSL;
using DataVector         = InstanceBuffer::DataVector;

// Since we update this before every draw call, it's stored as GL_DYNAMIC_DRAW
InstanceBuffer::InstanceBuffer()
	: ShaderStorageBuffer(3, sizeof(InstanceBufferGLSL), GL_DYNAMIC_DRAW)
{
	// Bind buffer
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, id);
	// Initialise empty data
	InstanceBufferGLSL instancedBuffer = {};
	// Buffer empty data
	glBufferSubData
	(
		GL_SHADER_STORAGE_BUFFER,
		static_cast<GLintptr>(0),
		static_cast<GLsizeiptr>(sizeof(InstanceBufferGLSL)),
		reinterpret_cast<const void*>(&instancedBuffer)
	);
	// Unbind buffer
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

void InstanceBuffer::LoadInstanceData(const EntityVector& entities)
{
	// Create data
	auto data = GenerateData(entities);
	// Buffer data
	glBufferSubData
	(
		GL_SHADER_STORAGE_BUFFER,
		static_cast<GLintptr>(0),
		static_cast<GLsizeiptr>(GetSize(entities)),
		reinterpret_cast<const void*>(&data[0])
	);
}

usize InstanceBuffer::GetSize(const EntityVector& entities)
{
	// Choose the one with the minimum size
	auto length = std::min(entities.size(), NUM_MAX_ENTITIES);
	// Calculate the size of the data
	auto size = length * sizeof(InstanceDataGLSL);
	// Return as GLsizeiptr
	return size;
}

DataVector InstanceBuffer::GenerateData(const EntityVector& entities)
{
	// Create data vector
	DataVector data = {};
	// Allocate data vector's memory
	data.reserve(GetSize(entities));

	// For each entity
	for (usize i = 0; i < entities.size(); ++i)
	{
		// Store model matrix
		data[i].modelMatrix = entities[i]->transform.GetModelMatrix();
		// Store normal matrix
		data[i].normalMatrix = glm::mat4(glm::transpose(glm::inverse(glm::mat3(data[i].modelMatrix))));
	}

	// Return created data
	return data;
}

void InstanceBuffer::Bind() const
{
	// Bind
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, id);
}

void InstanceBuffer::Unbind() const
{
	// Unbind
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}