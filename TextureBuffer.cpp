#include "Util.h"
#include "TextureBuffer.h"

// Using namespaces
using namespace Renderer;

// Aliases
using TextureBufferGLSL = TextureBuffer::TextureBufferGLSL;

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

    // For every texture
    for (const auto& texture : textures)
    {
        // Create texture set
        TextureSetGLSL set = {};
        set.albedoMap   = texture.albedo->handle;
        set.normalMap   = texture.normal->handle;
        set.aoRghMtlMap = texture.aoRghMtl->handle;
        set.emmisiveMap = texture.emmisive->handle;
        // Add to vector
        handles.emplace_back(set);
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