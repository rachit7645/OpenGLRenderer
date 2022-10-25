#include "SSAOBuffers.h"

#include "Random.h"
#include "Maths.h"
#include "Window.h"
#include "FBOAttachment.h"

using namespace Renderer;

using Vec3s = SSAOBuffers::Vec3s;

constexpr glm::ivec2 NOISE_RESOLUTION = {4, 4};

SSAOBuffers::SSAOBuffers()
	: ssaoBuffer(std::make_shared<FrameBuffer>()),
	  blurBuffer(std::make_shared<FrameBuffer>()),
	  noiseBuffer(std::make_shared<Texture>())
{
	// Shared color attachment
	Renderer::FBOAttachment color0
	{
		GL_NEAREST,
		GL_NEAREST,
		GL_REPEAT,
		GL_RED,
		GL_RED,
		GL_FLOAT,
		GL_COLOR_ATTACHMENT0
	};
	// Shared depth attachment
	Renderer::FBOAttachment depth = {};
	{
		depth.internalFormat = GL_DEPTH_COMPONENT24;
		depth.slot           = GL_DEPTH_ATTACHMENT;
	}

	// Create ssao buffer
	ssaoBuffer->width  = Window::DIMENSIONS.x;
	ssaoBuffer->height = Window::DIMENSIONS.y;

	ssaoBuffer->CreateFrameBuffer();
	ssaoBuffer->Bind();
	ssaoBuffer->AddTexture(ssaoBuffer->colorTextures[0], color0);
	ssaoBuffer->CheckStatus();
	ssaoBuffer->EnableDepth();
	ssaoBuffer->Unbind();

	// Create blur buffer
	blurBuffer->width  = Window::DIMENSIONS.x;
	blurBuffer->height = Window::DIMENSIONS.y;

	blurBuffer->CreateFrameBuffer();
	blurBuffer->Bind();
	blurBuffer->AddTexture(blurBuffer->colorTextures[0], color0);
	blurBuffer->AddBuffer(blurBuffer->depthRenderBuffer, depth);
	blurBuffer->CheckStatus();
	blurBuffer->EnableDepth();
	blurBuffer->Unbind();

	// Create noise buffer
	noiseBuffer->width  = NOISE_RESOLUTION.x;
	noiseBuffer->height = NOISE_RESOLUTION.y;
	noiseBuffer->type   = GL_TEXTURE_2D;

	noiseBuffer->CreateTexture();
	noiseBuffer->Bind();
	noiseBuffer->SetParameter(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	noiseBuffer->SetParameter(GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	noiseBuffer->SetParameter(GL_TEXTURE_WRAP_S,     GL_REPEAT);
	noiseBuffer->SetParameter(GL_TEXTURE_WRAP_T,     GL_REPEAT);

	auto noiseKernel = GetNoiseKernel(NOISE_RESOLUTION.x * NOISE_RESOLUTION.y);
	noiseBuffer->LoadImageData
	(
		reinterpret_cast<u8*>(&noiseKernel[0][0]),
		GL_RGBA32F,
		GL_RGBA,
		GL_FLOAT
	);

	noiseBuffer->Unbind();
}

Vec3s SSAOBuffers::GetSampleKernel(usize size)
{
	Vec3s sampleKernel;

	for (usize i = 0; i < size; ++i)
	{
		// Create Sample
		auto sample = glm::vec3
		(
			Util::RandRange(0.0f, 1.0f) * 2.0f - 1.0f,
			Util::RandRange(0.0f, 1.0f) * 2.0f - 1.0f,
			Util::RandRange(0.0f, 1.0f)
		);
		// Normalise
		sample = glm::normalize(sample);
		sample *= Util::RandRange(0.0f, 1.0f);
		// Scale
		f32 scale  = static_cast<f32>(i) / static_cast<f32>(size);
		scale      = Maths::Lerp(0.1f, 1.0f, scale * scale);
		sample    *= scale;
		// Put into sample vector
		sampleKernel.emplace_back(sample);
	}

	return sampleKernel;
}

Vec3s SSAOBuffers::GetNoiseKernel(usize size)
{
	Vec3s noiseKernel;

	for (usize i = 0; i < size; i++)
	{
		// Create noise
		auto noise = glm::vec3
		(
			Util::RandRange(0.0f, 1.0f) * 2.0 - 1.0,
			Util::RandRange(0.0f, 1.0f) * 2.0 - 1.0,
			0.0f
		);
		// Put it in the vector
		noiseKernel.emplace_back(noise);
	}

	return noiseKernel;
}

void SSAOBuffers::BindSSAOBuffer() const
{
	ssaoBuffer->Bind();
}

void SSAOBuffers::BindBlurBuffer() const
{
	blurBuffer->Bind();
}

void SSAOBuffers::BindDefaultFBO() const
{
	ssaoBuffer->Unbind();
}