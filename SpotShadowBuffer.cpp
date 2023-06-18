#include "SpotShadowBuffer.h"

// Using namespaces
using namespace Renderer;

// Aliases
using SpotShadowBufferGLSL = SpotShadowBuffer::SpotShadowBufferGLSL;

SpotShadowBuffer::SpotShadowBuffer()
    : UniformBuffer(6, sizeof(SpotShadowBufferGLSL), GL_STATIC_DRAW)
{
    // Bind buffer
    glBindBuffer(GL_UNIFORM_BUFFER, id);
    // Initialise empty data
    SpotShadowBufferGLSL shadowBuffer = {};
    // Buffer empty data
    glBufferSubData
    (
        GL_UNIFORM_BUFFER,
        static_cast<GLintptr>(0),
        static_cast<GLsizeiptr>(sizeof(SpotShadowBufferGLSL)),
        reinterpret_cast<const void*>(&shadowBuffer)
    );
    // Unbind buffer
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void SpotShadowBuffer::LoadShadowMap(usize lightIndex, const glm::mat4& shadowMatrix)
{
    // Temporary shadow data
    SpotShadowBufferGLSL shadowBuffer = {};

    // Set matrix
    shadowBuffer.shadowMatrix = shadowMatrix;
    // Set light index
    shadowBuffer.lightIndex = {static_cast<GLint>(lightIndex)};

    // Bind UBO
    glBindBuffer(GL_UNIFORM_BUFFER, id);

    // Buffer shadow map data
    glBufferSubData
    (
        GL_UNIFORM_BUFFER,
        static_cast<GLintptr>(0),
        static_cast<GLsizeiptr>(sizeof(SpotShadowBufferGLSL)),
        reinterpret_cast<const void*>(&shadowBuffer)
    );

    // Unbind UBO
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}