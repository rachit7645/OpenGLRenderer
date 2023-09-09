#include "InstanceBuffer.h"

#include "Util.h"
#include "TextureBuffer.h"

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
    // Bind buffer
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, id);
    // Create data
    auto data = GenerateData(entities);
    // Get count
    auto count = GL::Int{static_cast<GLint>(GetCount(entities))};
    // Buffer count
    glBufferSubData
    (
        GL_SHADER_STORAGE_BUFFER,
        static_cast<GLintptr>(offsetof(InstanceBufferGLSL, instanceCount)),
        static_cast<GLsizeiptr>(sizeof(GL::Int)),
        reinterpret_cast<const void*>(&count)
    );
    // Buffer data
    glBufferSubData
    (
        GL_SHADER_STORAGE_BUFFER,
        static_cast<GLintptr>(offsetof(InstanceBufferGLSL, instances)),
        static_cast<GLsizeiptr>(GetSize(entities)),
        reinterpret_cast<const void*>(&data[0])
    );
    // Unbind buffer
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

usize InstanceBuffer::GetCount(const InstanceBuffer::EntityVector& entities)
{
    // Choose the one with the minimum size
    return std::min(entities.size(), NUM_MAX_ENTITIES);
}

usize InstanceBuffer::GetSize(const EntityVector& entities)
{
    // Calculate the size of the data
    return GetCount(entities) * sizeof(InstanceDataGLSL);
}

DataVector InstanceBuffer::GenerateData(const EntityVector& entities)
{
    // Create data vector
    DataVector data = {};
    // Allocate data vector's memory
    data.reserve(GetCount(entities));

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

void InstanceBuffer::Unbind()
{
    // Unbind
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}