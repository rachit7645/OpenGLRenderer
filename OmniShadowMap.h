#ifndef OMNI_SHADOW_MAP_H
#define OMNI_SHADOW_MAP_H

#include <memory>

#include "FrameBuffer.h"
#include "GLM.h"
#include "OmniShadowBuffer.h"

namespace Renderer
{
    class OmniShadowMap
    {
    public:
        // Usings
        using FbPtr  = std::shared_ptr<FrameBuffer>;
        using MtxPtr = std::shared_ptr<OmniShadowBuffer>;
        using Mat4s  = std::vector<glm::mat4>;

        // Main constructor
        OmniShadowMap();

        // Update matrices
        void Update(usize lightIndex, const glm::vec3& lightPos);

        // Bind shadow cube map
        void BindShadowCubeMap() const;
        // Unbind shadow cube map
        void BindDefaultFBO() const;

        // Shadow cube map
        FbPtr shadowCubeMap;
    private:
        // Shadow matrix buffer
        MtxPtr m_matrixBuffer;
    };
}

#endif
