#include "OmniShadowMap.h"

#include "ShadowBuffer.h"
#include "Settings.h"

// Using namespaces
using namespace Renderer;

// Usings
using Engine::Settings;

// Constants
constexpr glm::vec2 SHADOW_PLANES = {1.0f, 25.0f};

OmniShadowMap::OmniShadowMap()
    : shadowCubeMap(std::make_shared<FrameBuffer>()),
      m_matrixBuffer(std::make_shared<OmniShadowBuffer>())
{
    // Get settings
    const auto& settings = Settings::GetInstance();

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
    shadowCubeMap->width  = settings.pointShadows.dimensions.x;
    shadowCubeMap->height = settings.pointShadows.dimensions.y;
    shadowCubeMap->depth  = static_cast<GLsizei>(SHADER_MAX_LIGHTS);

    // Create frame buffer object
    shadowCubeMap->CreateFrameBuffer();
    shadowCubeMap->Bind();
    // Set color buffer to none
    shadowCubeMap->SetDrawBuffer(GL_NONE);
    shadowCubeMap->SetReadBuffer(GL_NONE);
    // Create depth cube map
    shadowCubeMap->AddTextureCubeMapArray(shadowCubeMap->depthTexture, depth);
    // Finish up
    shadowCubeMap->CheckStatus();
    shadowCubeMap->EnableDepth();
    shadowCubeMap->Unbind();
}

void OmniShadowMap::Update(usize lightIndex, const glm::vec3& lightPos)
{
    // Get settings
    const auto& settings = Settings::GetInstance();

    // Shadow projection matrix
    glm::mat4 shadowProj = glm::perspective
    (
        glm::radians(90.0f),
        static_cast<f32>(settings.pointShadows.dimensions.x) / static_cast<f32>(settings.pointShadows.dimensions.y),
        SHADOW_PLANES.x,
        SHADOW_PLANES.y
    );

    // Shadow matrix vector
    Mat4s shadowMatrices =
    {
        // Fill shadow matrices for each face
        shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
        shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
        shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3( 0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)),
        shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3( 0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)),
        shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3( 0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
        shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3( 0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f))
    };

    // Upload data
    m_matrixBuffer->LoadShadowMap(shadowMatrices, SHADOW_PLANES, lightIndex);
    // Upload current light index
    m_matrixBuffer->LoadCurrentLightIndex(lightIndex);
}

void OmniShadowMap::BindShadowCubeMap() const
{
    // Bind
    shadowCubeMap->Bind();
}

void OmniShadowMap::BindDefaultFBO() const
{
    // Unbind
    shadowCubeMap->Unbind();
}