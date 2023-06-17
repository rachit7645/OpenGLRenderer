#include "LightsBuffer.h"

#include <GL/glew.h>

#include "Util.h"

// Using namespaces
using namespace Renderer;

// Usings
using Entities::DirectionalLight;
using Entities::PointLight;
using Entities::SpotLight;

// Aliases
using LightsBufferGLSL = LightsBuffer::LightsBufferGLSL;

LightsBuffer::LightsBuffer()
    : UniformBuffer(1, sizeof(LightsBufferGLSL), GL_STATIC_DRAW)
{
    // Bind buffer
    glBindBuffer(GL_UNIFORM_BUFFER, id);
    // Initialise empty data
    LightsBufferGLSL lightsBuffer = {};
    // Buffer empty data
    glBufferSubData
    (
        GL_UNIFORM_BUFFER,
        static_cast<GLintptr>(0),
        static_cast<GLsizeiptr>(sizeof(LightsBufferGLSL)),
        reinterpret_cast<const void*>(&lightsBuffer)
    );
    // Unbind buffer
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void LightsBuffer::LoadDirectionalLights(const std::vector<DirectionalLight>& lights)
{
    // Data
    usize   size         = std::min(lights.size(), SHADER_MAX_LIGHTS);
    GL::Int numDirLights = {static_cast<GLint>(size)};

    // Bind buffer
    glBindBuffer(GL_UNIFORM_BUFFER, id);

    // Load number of lights
    glBufferSubData
    (
        GL_UNIFORM_BUFFER,
        static_cast<GLintptr>(offsetof(LightsBufferGLSL, numDirLights)),
        static_cast<GLsizeiptr>(sizeof(GL::Int)),
        reinterpret_cast<const void*>(&numDirLights)
    );

    // Load lights
    glBufferSubData
    (
        GL_UNIFORM_BUFFER,
        static_cast<GLintptr>(offsetof(LightsBufferGLSL, dirLights)),
        static_cast<GLsizeiptr>(sizeof(DirectionalLight) * size),
        reinterpret_cast<const void*>(&lights[0])
    );

    // Unbind buffer
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void LightsBuffer::LoadPointLights(const std::vector<PointLight>& lights)
{
    // Data
    usize   size           = std::min(lights.size(), SHADER_MAX_LIGHTS);
    GL::Int numPointLights = {static_cast<GLint>(size)};

    // Bind buffer
    glBindBuffer(GL_UNIFORM_BUFFER, id);

    // Load number of lights
    glBufferSubData
    (
        GL_UNIFORM_BUFFER,
        static_cast<GLintptr>(offsetof(LightsBufferGLSL, numPointLights)),
        static_cast<GLsizeiptr>(sizeof(GL::Int)),
        reinterpret_cast<const void*>(&numPointLights)
    );

    // Load lights
    glBufferSubData
    (
        GL_UNIFORM_BUFFER,
        static_cast<GLintptr>(offsetof(LightsBufferGLSL, pointLights)),
        static_cast<GLsizeiptr>(sizeof(PointLight) * size),
        reinterpret_cast<const void*>(&lights[0])
    );

    // Unbind buffer
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void LightsBuffer::LoadSpotLights(const std::vector<SpotLight>& lights)
{
    // Data
    usize   size          = std::min(lights.size(), SHADER_MAX_LIGHTS);
    GL::Int numSpotLights = {static_cast<GLint>(size)};

    // Bind buffer
    glBindBuffer(GL_UNIFORM_BUFFER, id);

    // Load number of lights
    glBufferSubData
    (
        GL_UNIFORM_BUFFER,
        static_cast<GLintptr>(offsetof(LightsBufferGLSL, numSpotLights)),
        static_cast<GLsizeiptr>(sizeof(GL::Int)),
        reinterpret_cast<const void*>(&numSpotLights)
    );

    // Load lights
    glBufferSubData
    (
        GL_UNIFORM_BUFFER,
        static_cast<GLintptr>(offsetof(LightsBufferGLSL, spotLights)),
        static_cast<GLsizeiptr>(sizeof(SpotLight) * size),
        reinterpret_cast<const void*>(&lights[0])
    );

    // Unbind buffer
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}