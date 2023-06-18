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

void OmniShadowBuffer::LoadShadowMap(const Mat4s& matrices, glm::vec2 shadowPlanes, usize lightIndex)
{
    // Temporary shadow data
    PointShadowGLSL shadowMap = {};

    // Calculate number of matrices
    auto numMatrices = std::min(matrices.size(), SHADOW_CUBE_MAP_FACES);

    // For each matrix
    for (usize i = 0; i < numMatrices; ++i)
    {
        // Copy matrix
        shadowMap.matrices[i] = matrices[i];
    }

    // Convert planes
    shadowMap.shadowPlanes = glm::vec4(shadowPlanes, 1.0f, 1.0f);
    // Convert index
    shadowMap.lightIndex = {static_cast<GLint>(lightIndex)};

    // Bind UBO
    glBindBuffer(GL_UNIFORM_BUFFER, id);

    // Buffer shadow map data
    glBufferSubData
    (
        GL_UNIFORM_BUFFER,
        static_cast<GLintptr>(offsetof(OmniShadowBufferGLSL, omniShadowMaps[0]) + sizeof(PointShadowGLSL) * lightIndex),
        static_cast<GLsizeiptr>(sizeof(PointShadowGLSL)),
        reinterpret_cast<const void*>(&shadowMap)
    );

    // Unbind UBO
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void OmniShadowBuffer::LoadCurrentLightIndex(usize lightIndex)
{
    // Convert index
    GL::Int indexGL = {static_cast<GLint>(lightIndex)};

    // Bind UBO
    glBindBuffer(GL_UNIFORM_BUFFER, id);

    // Load to UBO
    glBufferSubData
    (
        GL_UNIFORM_BUFFER,
        static_cast<GLintptr>(offsetof(OmniShadowBufferGLSL, currentIndex)),
        static_cast<GLsizeiptr>(sizeof(GL::Int)),
        reinterpret_cast<const void*>(&indexGL)
    );

    // Unbind UBO
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}