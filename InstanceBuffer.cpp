#include "InstanceBuffer.h"

#include "Util.h"
#include "TextureBuffer.h"

// Using namespaces
using namespace Renderer;

// Usings
using Entities::Entity;

// Aliases
using InstanceBufferGLSL = InstanceBuffer::InstanceBufferGLSL;
using DataVector         = InstanceBuffer::DataVector;

// Since we update this before every draw call, it's stored as GL_DYNAMIC_DRAW
InstanceBuffer::InstanceBuffer()
    : ShaderStorageBuffer(3, sizeof(InstanceBufferGLSL), GL_DYNAMIC_DRAW)
{
    // Bind buffer
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, id);
    // Initialise empty data
    auto instancedBuffer = std::make_unique<InstanceBufferGLSL>();
    // Buffer empty data
    glBufferSubData
    (
        GL_SHADER_STORAGE_BUFFER,
        static_cast<GLintptr>(0),
        static_cast<GLsizeiptr>(sizeof(InstanceBufferGLSL)),
        reinterpret_cast<const void*>(instancedBuffer.get())
    );
    // Unbind buffer
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

void InstanceBuffer::LoadInstanceData(const Batch& batch)
{
    // Create data
    auto data = GenerateData(batch);

    // Bind buffer
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, id);

    // Buffer data
    glBufferSubData
    (
        GL_SHADER_STORAGE_BUFFER,
        static_cast<GLintptr>(offsetof(InstanceBufferGLSL, instances)),
        static_cast<GLsizeiptr>(GetSize(batch)),
        reinterpret_cast<const void*>(&data[0])
    );

    // Unbind buffer
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

DataVector InstanceBuffer::GenerateData(const Batch& batch)
{
    // Create data vector
    DataVector data = {};
    // Allocate data vector's memory
    data.reserve(GetCount(batch));

    // For each entity group
    for (const auto& [model, entities] : batch)
    {
        // For each entity
        for (const auto& entity : entities)
        {
            // For each mesh in each entity // Will be useful for animations
            for (UNUSED const auto& mesh : model->meshes)
            {
                // Add instance data
                InstanceDataGLSL instance = {};
                instance.modelMatrix  = entity->transform.GetModelMatrix();
                instance.normalMatrix = glm::mat4(glm::transpose(glm::inverse(glm::mat3(instance.modelMatrix))));
                data.emplace_back(instance);
            }
        }
    }

    // Return created data
    return data;
}

usize InstanceBuffer::GetCount(const Batch& batch)
{
    // Total entity count
    usize total = 0;

    // For all entity groups
    for (const auto& [model, entities] : batch)
    {
        // Add entity mesh count
        total += entities.size() * model->meshes.size();
    }

    // Check for space
    return std::min(total, NUM_MAX_ENTITIES);
}

usize InstanceBuffer::GetSize(const Batch& batch)
{
    // Calculate the size of the data
    return GetCount(batch) * sizeof(InstanceDataGLSL);
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