#include "OmniShadowBuffer.h"

// Usings
using namespace Renderer;

// Aliases
using OmniShadowBufferGLSL = OmniShadowBuffer::OmniShadowBufferGLSL;
using Mat4s                = OmniShadowBuffer::Mat4s;

OmniShadowBuffer::OmniShadowBuffer()
    : UniformBuffer(5, sizeof(OmniShadowBufferGLSL), GL_STATIC_DRAW)
{
    // Bind buffer
    glBindBuffer(GL_UNIFORM_BUFFER, id);
    // Initialise empty data
    OmniShadowBufferGLSL shadowBuffer = {};
    // Buffer empty data
    glBufferSubData
    (
        GL_UNIFORM_BUFFER,
        static_cast<GLintptr>(0),
        static_cast<GLsizeiptr>(sizeof(OmniShadowBufferGLSL)),
        reinterpret_cast<const void*>(&shadowBuffer)
    );
    // Unbind buffer
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void OmniShadowBuffer::LoadMatrices(const Mat4s& matrices)
{
    // Calculate size
    auto size = std::min(matrices.size(), SHADOW_CUBE_MAP_FACES) * sizeof(glm::mat4);

    // Bind UBO
    glBindBuffer(GL_UNIFORM_BUFFER, id);

    // Buffer matrix data
    glBufferSubData
    (
        GL_UNIFORM_BUFFER,
        static_cast<GLintptr>(offsetof(OmniShadowBufferGLSL, matrices)),
        static_cast<GLsizeiptr>(size),
        reinterpret_cast<const void*>(&matrices[0])
    );

    // Unbind UBO
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void OmniShadowBuffer::LoadShadowPlanes(const glm::vec2& planes)
{
    // Convert
    glm::vec4 glPlanes = glm::vec4(planes, 1.0f, 1.0f);

    // Bind UBO
    glBindBuffer(GL_UNIFORM_BUFFER, id);

    // Buffer index
    glBufferSubData
    (
        GL_UNIFORM_BUFFER,
        static_cast<GLintptr>(offsetof(OmniShadowBufferGLSL, shadowPlanes)),
        static_cast<GLsizeiptr>(sizeof(glm::vec4)),
        reinterpret_cast<const void*>(&glPlanes[0])
    );

    // Unbind UBO
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void OmniShadowBuffer::LoadLightIndex(usize index)
{
    // Convert
    GL::Int glIndex = {static_cast<GLint>(index)};

    // Bind UBO
    glBindBuffer(GL_UNIFORM_BUFFER, id);

    // Buffer index
    glBufferSubData
    (
        GL_UNIFORM_BUFFER,
        static_cast<GLintptr>(offsetof(OmniShadowBufferGLSL, lightIndex)),
        static_cast<GLsizeiptr>(sizeof(GL::Int)),
        reinterpret_cast<const void*>(&glIndex)
    );

    // Unbind UBO
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}