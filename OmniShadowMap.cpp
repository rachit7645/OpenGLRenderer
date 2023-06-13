#include "OmniShadowMap.h"

#include "ShadowBuffer.h"

// Using namespaces
using namespace Renderer;

// Constants
constexpr glm::ivec2 SHADOW_DIMENSIONS = {1024, 1024};

OmniShadowMap::OmniShadowMap()
    : shadowCubeMap(std::make_shared<FrameBuffer>())
{
    // Make sure enough space is available on the GPU
    static_assert(SHADOW_MAX_FRUSTUMS >= 6, "We assume at least 6 shadow matrix slots are available!");

    // Depth attachment
    Renderer::FBOAttachment depth =
    {
        GL_NEAREST,
        GL_NEAREST,
        GL_CLAMP_TO_BORDER,
        GL_DEPTH_COMPONENT24,
        GL_DEPTH_COMPONENT,
        GL_FLOAT,
        GL_DEPTH_ATTACHMENT,
        nullptr
    };

    // Set buffer dimensions
    shadowCubeMap->width  = SHADOW_DIMENSIONS.x;
    shadowCubeMap->height = SHADOW_DIMENSIONS.y;

    // Create frame buffer object
    shadowCubeMap->CreateFrameBuffer();
    shadowCubeMap->Bind();
    // Set color buffer to none
    shadowCubeMap->SetDrawBuffer(GL_NONE);
    shadowCubeMap->SetReadBuffer(GL_NONE);
    // Create depth cube map
    shadowCubeMap->AddTextureCubeMap(shadowCubeMap->depthTexture, depth);
    // Finish up
    shadowCubeMap->CheckStatus();
    shadowCubeMap->EnableDepth();
    shadowCubeMap->Unbind();
}

void OmniShadowMap::Bind() const
{
    // Bind
    shadowCubeMap->Bind();
}

void OmniShadowMap::Unbind() const
{
    // Unbind
    shadowCubeMap->Unbind();
}