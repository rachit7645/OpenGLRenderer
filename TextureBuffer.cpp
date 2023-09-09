#include "Util.h"
#include "InstanceBuffer.h"
#include "TextureBuffer.h"
#include "Log.h"

// Using namespaces
using namespace Renderer;

// Aliases
using TextureSetGLSL    = TextureBuffer::TextureSetGLSL;
using TextureBufferGLSL = TextureBuffer::TextureBufferGLSL;
using TexturesGLSL      = TextureBuffer::Textures;

TextureBuffer::TextureBuffer()
    : ShaderStorageBuffer(7, sizeof(TextureBufferGLSL), GL_DYNAMIC_DRAW)
{
    // Bind buffer
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, id);
    // Initialise empty data
    TextureBufferGLSL textureBuffer = {};
    // Buffer empty data
    glBufferSubData
    (
        GL_SHADER_STORAGE_BUFFER,
        static_cast<GLintptr>(0),
        static_cast<GLsizeiptr>(sizeof(TextureBufferGLSL)),
        reinterpret_cast<const void*>(&textureBuffer)
    );
    // Unbind buffer
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

void TextureBuffer::LoadTextures(const Textures& textures)
{
    // Calculate size
    usize size = std::min(textures.size(), SHADER_MAX_TEXTURE_SETS);

    // Texture handle vector
    std::vector<TextureSetGLSL> handles = {};
    // Reserve memory
    handles.reserve(size);

    // For every texture (in the EXACT same order)
    for (usize i = 0; i < size; ++i)
    {
        // Get texture
        const auto& set = textures[i];
        // Pack handles
        handles[i] =
        {
            set.albedo->handle,
            set.normal->handle,
            set.aoRghMtl->handle,
            set.emmisive->handle
        };
    }

    // Bind buffer
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, id);

    // Load textures
    glBufferSubData
    (
        GL_SHADER_STORAGE_BUFFER,
        static_cast<GLintptr>(offsetof(TextureBufferGLSL, sets)),
        static_cast<GLsizeiptr>(sizeof(TextureSetGLSL) * size),
        reinterpret_cast<const void*>(&handles[0])
    );

    // Unbind buffer
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

void TextureBuffer::SetIndex(usize index)
{
    // Bind buffer
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, id);
    // Empty index
    GL::Int glIndex = {static_cast<GLint>(index)};
    // Buffer data
    glBufferSubData
    (
        GL_SHADER_STORAGE_BUFFER,
        static_cast<GLintptr>(offsetof(TextureBufferGLSL, currentIndex)),
        static_cast<GLsizeiptr>(sizeof(GL::Int)),
        reinterpret_cast<const void*>(&glIndex)
    );
    // Unbind buffer
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}