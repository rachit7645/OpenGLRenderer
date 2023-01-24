#include "SSAOBuffers.h"
#include "Settings.h"
#include "Util.h"
#include "Random.h"
#include "Maths.h"

// Using namespaces
using namespace Renderer;

// Usings
using Engine::Settings;

// Constants
constexpr usize NUM_SAMPLES = 6;

SSAOBuffers::SSAOBuffers()
	: ssaoBuffer(std::make_shared<FrameBuffer>()),
	  ssaoBlurBuffer(std::make_shared<FrameBuffer>()),
	  noiseTexture(std::make_shared<Texture>()),
	  m_ssaoBuffer(std::make_shared<SSAOBuffer>())
{
	// Init FBOs
	InitBuffers();
	// Create noise data
	CreateNoiseTexture();
	// Load sample kernels
	LoadKernels();
}

void SSAOBuffers::InitBuffers()
{
	// Get settings instance
	const auto& settings = Settings::GetInstance();

	// Occlusion attachment
	Renderer::FBOAttachment occlusion =
	{
		GL_NEAREST,
		GL_NEAREST,
		GL_CLAMP_TO_EDGE,
		GL_RED,
		GL_RED,
		GL_FLOAT,
		GL_COLOR_ATTACHMENT0
	};

	// Stencil attachment
	Renderer::FBOAttachment stencil = {};
	{
		stencil.intFormat = GL_STENCIL_INDEX8;
		stencil.slot      = GL_STENCIL_ATTACHMENT;
	}

	// Draw buffers
	std::vector<GLenum> drawBuffers =
	{
		occlusion.slot
	};

	// Set SSAO buffer dimensions
	ssaoBuffer->width  = settings.window.dimensions.x;
	ssaoBuffer->height = settings.window.dimensions.y;

	// Create SSAO buffer
	ssaoBuffer->CreateFrameBuffer();
	ssaoBuffer->Bind();
	ssaoBuffer->AddTexture(ssaoBuffer->colorTextures[0], occlusion);
	ssaoBuffer->AddBuffer(ssaoBuffer->stencilRenderBuffer, stencil);
	ssaoBuffer->SetDrawBuffers(drawBuffers);
	ssaoBuffer->CheckStatus();
	ssaoBuffer->Unbind();

	// Set SSAO blur buffer dimensions
	ssaoBlurBuffer->width  = settings.window.dimensions.x;
	ssaoBlurBuffer->height = settings.window.dimensions.y;

	// Create SSAO buffer
	ssaoBlurBuffer->CreateFrameBuffer();
	ssaoBlurBuffer->Bind();
	ssaoBlurBuffer->AddTexture(ssaoBlurBuffer->colorTextures[0], occlusion);
	ssaoBlurBuffer->AddBuffer(ssaoBlurBuffer->stencilRenderBuffer, stencil);
	ssaoBlurBuffer->SetDrawBuffers(drawBuffers);
	ssaoBlurBuffer->CheckStatus();
	ssaoBlurBuffer->Unbind();
}

void SSAOBuffers::CreateNoiseTexture()
{
	// Create noise data
	std::vector<glm::vec3> ssaoNoise;
	// Allocate memory for noise data
	ssaoNoise.reserve(16);

	// For each texel
	for (usize i = 0; i < 16; ++i)
	{
		// Add noise data
		ssaoNoise.emplace_back
		(
			Util::RandRange(0.0f, 1.0f) * 2.0f - 1.0f,
			Util::RandRange(0.0f, 1.0f) * 2.0f - 1.0f,
      	  	0.0f
		);
	}

	// Set noise dimensions
	noiseTexture->width  = 4;
	noiseTexture->height = 4;
	// Create noise texture
	noiseTexture->CreateTexture();
	noiseTexture->Bind();
	// Set parameters
	noiseTexture->SetParameter(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	noiseTexture->SetParameter(GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	noiseTexture->SetParameter(GL_TEXTURE_WRAP_S,     GL_REPEAT);
	noiseTexture->SetParameter(GL_TEXTURE_WRAP_T,     GL_REPEAT);
	// Load noise data
	noiseTexture->LoadImageData(reinterpret_cast<u8*>(ssaoNoise.data()), GL_RGB16F, GL_RGB, GL_FLOAT);
	// Unbind texture
	noiseTexture->Unbind();
}

void SSAOBuffers::LoadKernels()
{
	// Create kernel sample data
	std::vector<glm::vec4> ssaoKernel;
	// Allocate memory for data
	ssaoKernel.reserve(NUM_SAMPLES);

	// For each kernel
	for (usize i = 0; i < NUM_SAMPLES; ++i)
	{
		// Create sample
		auto sample = glm::vec3
		(
			Util::RandRange(0.0f, 1.0f) * 2.0f - 1.0f,
			Util::RandRange(0.0f, 1.0f) * 2.0f - 1.0f,
			Util::RandRange(0.0f, 1.0f)
		);
		// Normalise sample
		sample = glm::normalize(sample);
		// Offset sample
		sample *= Util::RandRange(0.0f, 1.0f);

		// Get scale factor
		f32 scale = static_cast<f32>(i) / static_cast<f32>(NUM_SAMPLES);
		// Scale samples to align them to the center
		scale = Maths::Lerp(0.1f, 1.0f, scale * scale);
		// Offset sample by scale
		sample *= scale;

		// Add sample to kernel
		ssaoKernel.emplace_back(sample, 1.0f);
	}

	// Load to GPU
	m_ssaoBuffer->LoadKernels(ssaoKernel);
}

void SSAOBuffers::BindSSAOBuffer() const
{
	// Bind SSAO buffer
	ssaoBuffer->Bind();
}

void SSAOBuffers::BindSSAOBlurBuffer() const
{
	// Bind SSAO blur buffer
	ssaoBlurBuffer->Bind();
}

void SSAOBuffers::BindDefaultFBO() const
{
	// Unbind
	ssaoBuffer->Unbind();
}