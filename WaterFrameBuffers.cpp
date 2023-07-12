#include "WaterFrameBuffers.h"

#include "FBOAttachment.h"
#include "Settings.h"

// Using namespaces
using namespace Waters;

// Usings
using Renderer::FrameBuffer;
using Engine::Settings;

WaterFrameBuffers::WaterFrameBuffers()
    : reflectionFBO(std::make_shared<FrameBuffer>()),
      refractionFBO(std::make_shared<FrameBuffer>())
{
    // Get settings instance
    const auto& settings = Settings::GetInstance();

    // Calculate new size
    glm::vec2 newSize = glm::vec2(settings.window.dimensions) * settings.water.multipliers;

    // Calculate sizes
    auto reflectionSize = glm::ivec2(newSize);
    auto refractionSize = glm::ivec2(newSize);

    // Color attachment
    Renderer::FBOAttachment color0 =
    {
        GL_LINEAR,
        GL_LINEAR,
        GL_REPEAT,
        GL_RGB,
        GL_RGB,
        GL_UNSIGNED_BYTE,
        GL_COLOR_ATTACHMENT0
    };

    // Depth attachment
    Renderer::FBOAttachment depth = {};
    {
        depth.intFormat = GL_DEPTH_COMPONENT24;
        depth.slot      = GL_DEPTH_ATTACHMENT;
    }

    // Set reflection dimensions
    reflectionFBO->width  = reflectionSize.x;
    reflectionFBO->height = reflectionSize.y;

    // Create reflection FBO
    reflectionFBO->CreateFrameBuffer();
    reflectionFBO->Bind();
    reflectionFBO->AddTexture(reflectionFBO->colorTextures[0], color0);
    reflectionFBO->AddBuffer(reflectionFBO->depthRenderBuffer, depth);
    reflectionFBO->CheckStatus();
    reflectionFBO->EnableDepth();
    reflectionFBO->Unbind();

    // Set refraction dimensions
    refractionFBO->width  = refractionSize.x;
    refractionFBO->height = refractionSize.y;

    // Create refraction FBO
    refractionFBO->CreateFrameBuffer();
    refractionFBO->Bind();
    refractionFBO->AddTexture(refractionFBO->colorTextures[0], color0);
    refractionFBO->AddBuffer(refractionFBO->depthRenderBuffer, depth);
    refractionFBO->CheckStatus();
    refractionFBO->EnableDepth();
    refractionFBO->Unbind();
}

void WaterFrameBuffers::BindReflection() const
{
    // Bind
    reflectionFBO->Bind();
}

void WaterFrameBuffers::BindRefraction() const
{
    // Bind
    refractionFBO->Bind();
}

void WaterFrameBuffers::BindDefaultFBO() const
{
    // Unbind (We could also use the refraction FBO for this purpose)
    reflectionFBO->Unbind();
}