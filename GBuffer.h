#ifndef G_BUFFER_H
#define G_BUFFER_H

#include <memory>

#include "FrameBuffer.h"

namespace Renderer
{
    class GBuffer
    {
    public:
        using FbPtr = std::shared_ptr<FrameBuffer>;

        GBuffer();

        void BindGBuffer()    const;
        void BindDefaultFBO() const;

        FbPtr buffer;
    };
}

#endif
