#ifndef LIGHTING_BUFFER_H
#define LIGHTING_BUFFER_H

#include "FrameBuffer.h"

namespace Renderer
{
    class LightingBuffer
    {
    public:
        // Usings
        using FbPtr = std::shared_ptr<FrameBuffer>;

        // Main constructor
        LightingBuffer();

        // Bind FBO
        void BindLightingBuffer() const;
        // Unbind FBO
        void BindDefaultFBO()const;

        // Frame buffer
        FbPtr buffer;
    };
}

#endif
