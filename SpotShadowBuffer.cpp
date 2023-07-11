#include "SpotShadowBuffer.h"

// Using namespaces
using namespace Renderer;

// Aliases
using SpotShadowBufferGLSL = SpotShadowBuffer::SpotShadowBufferGLSL;
using SpotShadowGLSL       = SpotShadowBuffer::SpotShadowGLSL;

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
    SpotShadowGLSL spotShadow = {};

    // Set matrix
    spotShadow.shadowMatrix = shadowMatrix;
    // Set light index
    spotShadow.lightIndex = {static_cast<GLint>(lightIndex)};

    // Bind UBO
    glBindBuffer(GL_UNIFORM_BUFFER, id);

    // Buffer shadow map data
    glBufferSubData
    (
        GL_UNIFORM_BUFFER,
        static_cast<GLintptr>(offsetof(SpotShadowBufferGLSL, spotShadows[0]) + sizeof(SpotShadowGLSL) * lightIndex),
        static_cast<GLsizeiptr>(sizeof(SpotShadowGLSL)),
        reinterpret_cast<const void*>(&spotShadow)
    );

    // Unbind UBO
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void SpotShadowBuffer::LoadCurrentLightIndex(usize lightIndex)
{
    // Convert index
    GL::Int indexGL = {static_cast<GLint>(lightIndex)};

    // Bind UBO
    glBindBuffer(GL_UNIFORM_BUFFER, id);

    // Load to UBO
    glBufferSubData
    (
        GL_UNIFORM_BUFFER,
        static_cast<GLintptr>(offsetof(SpotShadowBufferGLSL, currentIndex)),
        static_cast<GLsizeiptr>(sizeof(GL::Int)),
        reinterpret_cast<const void*>(&indexGL)
    );

    // Unbind UBO
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}