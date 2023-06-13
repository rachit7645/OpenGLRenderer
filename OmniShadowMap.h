#ifndef OMNI_SHADOW_MAP_H
#define OMNI_SHADOW_MAP_H

#include <memory>

#include "FrameBuffer.h"
#include "GLM.h"

namespace Renderer
{
    class OmniShadowMap
    {
    public:
        // Usings
        using FbPtr = std::shared_ptr<FrameBuffer>;

        // Main constructor
        OmniShadowMap();

        // Bind shadow cube map
        void Bind() const;
        // Unbind shadow cube map
        void Unbind() const;

        // Shadow cube map
        FbPtr shadowCubeMap;
    };
}

#endif
