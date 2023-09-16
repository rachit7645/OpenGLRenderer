#ifndef TEXTURE_BUFFER_H
#define TEXTURE_BUFFER_H

#include <vector>
#include <memory>

#include "GLM.h"
#include "ShaderStorageBuffer.h"
#include "GL.h"
#include "Util.h"
#include "MeshTextures.h"

namespace Renderer
{
    // May adjust this later since its not very high
    constexpr usize SHADER_MAX_TEXTURE_SETS = 2048;

    class TextureBuffer : public ShaderStorageBuffer
    {
    public:
        // Aliases
        using Textures = std::vector<MeshTextures>;

        // GLSL implementation of a texture set
        struct ALIGN_GLSL_STD140 TextureSetGLSL
        {
            GLuint64 albedoMap   = {}; // Albedo
            GLuint64 normalMap   = {}; // Normal map
            GLuint64 aoRghMtlMap = {}; // AO + Roughness + Metallic
            GLuint64 emmisiveMap = {}; // Emmisive
        };

        // GLSL implementation struct
        struct ALIGN_GLSL_STD140 TextureBufferGLSL
        {
            // Array of texture sets
            TextureSetGLSL sets[SHADER_MAX_TEXTURE_SETS] = {};
        };

        // Main constructor
        TextureBuffer();

        // Loads textures into the buffer
        void LoadTextures(const Textures& textures);
    };
}

#endif
