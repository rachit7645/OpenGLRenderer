#include "SpotShadowMap.h"

#include "RenderConstants.h"

// Using namespaces
using namespace Renderer;

// Constants
constexpr glm::ivec2 SHADOW_DIMENSIONS = {1024, 1024};
constexpr glm::vec2  SHADOW_PLANES     = {0.1f, 300.0f};
constexpr glm::vec4  MAP_BORDER        = {1.0f, 1.0f, 1.0f, 1.0f};

SpotShadowMap::SpotShadowMap()
    : buffer(std::make_shared<FrameBuffer>()),
      m_matrixBuffer(std::make_shared<SpotShadowBuffer>())
{
    // Depth attachment
    Renderer::FBOAttachment depth =
    {
        GL_LINEAR,
        GL_LINEAR,
        GL_CLAMP_TO_BORDER,
        GL_DEPTH_COMPONENT24,
        GL_DEPTH_COMPONENT,
        GL_FLOAT,
        GL_DEPTH_ATTACHMENT,
        const_cast<f32*>(&MAP_BORDER[0])
    };

    // Set buffer dimensions
    buffer->width  = SHADOW_DIMENSIONS.x;
    buffer->height = SHADOW_DIMENSIONS.y;

    // Create frame buffer object
    buffer->CreateFrameBuffer();
    buffer->Bind();
    // Set color buffer to none
    buffer->SetDrawBuffer(GL_NONE);
    buffer->SetReadBuffer(GL_NONE);
    // Create depth texture
    buffer->AddTexture(buffer->depthTexture, depth);
    // Finish up
    buffer->CheckStatus();
    buffer->EnableDepth();
    buffer->Unbind();

    // Bind depth texture
    buffer->depthTexture->Bind();
    // Enable depth comparison mode
    buffer->depthTexture->SetParameter(GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
    // Set depth compare mode
    buffer->depthTexture->SetParameter(GL_TEXTURE_COMPARE_FUNC, GL_GREATER);
    // Unbind depth texture
    buffer->depthTexture->Bind();
}

void SpotShadowMap::Update(usize index, const glm::vec3& lightPos)
{
    // Create shadow projection matrix
    glm::mat4 projection = glm::perspective
    (
        glm::radians(90.0f),
        1.0f,
        SHADOW_PLANES.x,
        SHADOW_PLANES.y
    );

    // Create shadow view matrix
    glm::mat4 view = glm::lookAt
    (
        lightPos,
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f)
    );

    // Combine matrices
    glm::mat4 shadowMatrix = projection * view;

    // Load shadow data
    m_matrixBuffer->LoadShadowMap(index, shadowMatrix);
}