#ifndef SPOT_SHADOW_MAP_H
#define SPOT_SHADOW_MAP_H

#include <memory>

#include "GLM.h"
#include "FrameBuffer.h"
#include "SpotShadowBuffer.h"

namespace Renderer
{
    class SpotShadowMap
    {
    public:
        // Usings
        using FbPtr  = std::shared_ptr<FrameBuffer>;
        using MtxPtr = std::shared_ptr<SpotShadowBuffer>;

        // Main constructor
        SpotShadowMap();

        // Update shadow map
        void Update(usize index, const glm::vec3& lightPos);

        // Shadow buffer
        FbPtr buffer;
    private:
        MtxPtr m_matrixBuffer;
    };
}

#endif
