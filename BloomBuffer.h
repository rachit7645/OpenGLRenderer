#ifndef BLOOM_BUFFER_H
#define BLOOM_BUFFER_H

#include <memory>

#include "FrameBuffer.h"
#include "Texture.h"

namespace Renderer
{
    class BloomBuffer
    {
    public:
        // Usings
        using FbPtr = std::shared_ptr<FrameBuffer>;
        using TxPtr = std::shared_ptr<Texture>;

        // Main constructor
        BloomBuffer();

        // Bind FBO
        void BindBloomBuffer() const;
        // Unbind FBO
        void BindDefaultFBO() const;

        // Frame buffer
        FbPtr buffer;
        // MipMap chain
        std::vector<TxPtr> mipChain;
    };
}

#endif
