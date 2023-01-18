#include "IBLRenderer.h"

#include <vector>

// Using namespaces
using namespace Renderer;

// Usings
using Shader::ConverterShader;
using Shader::ConvolutionShader;
using Shader::PreFilterShader;
using Shader::BRDFShader;

// Aliases
using VAO   = IBLRenderer::VAO;
using TxPtr = IBLRenderer::TxPtr;
using FbPtr = IBLRenderer::FbPtr;

// Constants
constexpr usize      PRE_FILTER_MIPMAP_LEVELS = {5};
constexpr glm::ivec2 CUBEMAP_DIMENSIONS       = {1024, 1024};
constexpr glm::ivec2 IRRADIANCE_DIMENSIONS    = {32,     32};
constexpr glm::ivec2 PRE_FILTER_DIMENSIONS    = {128,   128};
constexpr glm::ivec2 BRDF_LUT_DIMENSIONS      = {512,   512};

IBLRenderer::IBLRenderer
(
	Shader::ConverterShader& converter,
	Shader::ConvolutionShader& convolution,
	Shader::PreFilterShader& preFilter,
	Shader::BRDFShader& brdf
)
	: converter(converter),
	  convolution(convolution),
	  preFilter(preFilter),
	  brdf(brdf),
	  m_projection(LoadProjection()),
	  m_views(LoadViews()),
	  m_cube(LoadCube()),
	  m_quad(LoadQuad())
{
	// Start converter shader
	converter.Start();
	// Connect texture units
	converter.ConnectTextureUnits();
	// Load projection
	converter.LoadProjection(m_projection);
	// Stop shader
	converter.Stop();

	// Start convolution shader
	convolution.Start();
	// Connect texture units
	convolution.ConnectTextureUnits();
	// Load projection
	convolution.LoadProjection(m_projection);
	// Stop shader
	convolution.Stop();

	// Start convolution shader
	preFilter.Start();
	// Connect texture units
	preFilter.ConnectTextureUnits();
	// Load projection
	preFilter.LoadProjection(m_projection);
	// Load resolution
	preFilter.LoadResolution(CUBEMAP_DIMENSIONS);
	// Stop shader
	preFilter.Stop();
}

void IBLRenderer::ConvertToCubeMap(TxPtr& cubeMap, TxPtr& hdrMap)
{
	// Create cube map FBO
	auto cubeMapFBO = CreateCubeMapFBO(CUBEMAP_DIMENSIONS, true);

	// Prepare
	PrepareRender(cubeMapFBO, m_cube);
	// Start shader
	converter.Start();
	// Bind HDR map
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, hdrMap->id);

	// For each face
	for (usize i = 0; i < m_views.size(); ++i)
	{
		// Load view matrix
		converter.LoadView(m_views[i]);
		// Render cube face
		RenderCubeFace(cubeMapFBO, i);
	}

	// Unbind
	UnbindRender(cubeMapFBO);
	// Stop shader
	converter.Stop();

	// Store rendered texture
	cubeMap = cubeMapFBO->colorTextures[0];

	// Generate mipmaps
	cubeMap->GenerateMipmaps();
}

void IBLRenderer::GenerateIrradiance(TxPtr& irradiance, TxPtr& cubeMap)
{
	// Create cube map FBO
	auto irradianceFBO = CreateCubeMapFBO(IRRADIANCE_DIMENSIONS);

	// Prepare
	PrepareRender(irradianceFBO, m_cube);
	// Start shader
	convolution.Start();
	// Bind environment map
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMap->id);

	// For each face
	for (usize i = 0; i < m_views.size(); ++i)
	{
		// Load view matrix
		convolution.LoadView(m_views[i]);
		// Render cube face
		RenderCubeFace(irradianceFBO, i);
	}

	// Unbind
	UnbindRender(irradianceFBO);
	// Stop shader
	convolution.Stop();

	// Store rendered texture
	irradiance = irradianceFBO->colorTextures[0];
}

void IBLRenderer::PreFilterSpecular(TxPtr& preFilterMap, TxPtr& cubeMap)
{
	// Create cube map FBO
	auto preFilterFBO = CreateCubeMapFBO(PRE_FILTER_DIMENSIONS, true);

	// Generate mipmaps
	preFilterFBO->colorTextures[0]->GenerateMipmaps();

	// Prepare
	PrepareRender(preFilterFBO, m_cube);
	// Start shader
	preFilter.Start();
	// Bind environment map
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMap->id);

	// For each mipmap level
	for (usize mip = 0; mip < PRE_FILTER_MIPMAP_LEVELS; ++mip)
	{
		// Calculate width and height
		auto mipWidth  = static_cast<GLint>(PRE_FILTER_DIMENSIONS.x * std::pow(0.5f, mip));
		auto mipHeight = static_cast<GLint>(PRE_FILTER_DIMENSIONS.y * std::pow(0.5f, mip));

		// Update framebuffer
		glBindRenderbuffer(GL_RENDERBUFFER, preFilterFBO->depthRenderBuffer->id);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, mipWidth, mipHeight);
		glViewport(0, 0, mipWidth, mipHeight);

		// Load roughness
		f32 roughness = static_cast<f32>(mip) / static_cast<f32>(PRE_FILTER_MIPMAP_LEVELS - 1);
		preFilter.LoadRoughness(roughness);

		// For each face
		for (usize i = 0; i < m_views.size(); ++i)
		{
			// Load view matrix
			preFilter.LoadView(m_views[i]);
			// Render cube face
			RenderCubeFace(preFilterFBO, i, static_cast<GLint>(mip));
		}
	}

	// Unbind
	UnbindRender(preFilterFBO);
	// Stop shader
	preFilter.Stop();

	preFilterMap = preFilterFBO->colorTextures[0];
}

void IBLRenderer::CalculateBRDF(TxPtr& brdfLut)
{
	// Create 2D FBO
	auto brdfFBO = Create2DFBO(BRDF_LUT_DIMENSIONS);

	// Prepare
	PrepareRender(brdfFBO, m_quad);
	// Start shader
	brdf.Start();
	// Draw
	RenderQuad();
	// Unbind
	UnbindRender(brdfFBO);
	// Stop shader
	brdf.Stop();

	// Store rendered texture
	brdfLut = brdfFBO->colorTextures[0];
}

void IBLRenderer::PrepareRender(FbPtr& FBO, const VAO& vao)
{
	// Bind
	FBO->Bind();
	// Set rasterizer state
	glDepthFunc(GL_LEQUAL);
	glDisable(GL_CULL_FACE);
	// Bind vao
	glBindVertexArray(vao->id);
}

void IBLRenderer::UnbindRender(FbPtr& FBO)
{
	// Reset rasterizer state
	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);
	// Unbind
	glBindVertexArray(0);
	FBO->Unbind();
}

void IBLRenderer::RenderCubeFace(FbPtr& FBO, usize face, GLint level)
{
	// Assign texture
	glFramebufferTexture2D
	(
		GL_FRAMEBUFFER,
		GL_COLOR_ATTACHMENT0,
		GL_TEXTURE_CUBE_MAP_POSITIVE_X + face,
		FBO->colorTextures[0]->id,
		level
	);
	// Clear FBO
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// Render cube
	glDrawArrays(GL_TRIANGLES, 0, m_cube->vertexCount);
}

void IBLRenderer::RenderQuad()
{
	// Set rasterizer state
	glDisable(GL_DEPTH_TEST);
	// Clear FBO
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	// Render quad
	glDrawArrays(GL_TRIANGLE_STRIP, 0, m_quad->vertexCount);
	// Reset rasterizer state
	glEnable(GL_DEPTH_TEST);
}

FbPtr IBLRenderer::CreateCubeMapFBO(const glm::ivec2& dimensions, bool isMipMapped)
{
	// Allocate FBO on CPU
	auto FBO = std::make_shared<FrameBuffer>();

	// Color attachment
	Renderer::FBOAttachment color0 =
	{
		isMipMapped ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR,
		GL_LINEAR,
		GL_CLAMP_TO_EDGE,
		GL_RGB16F,
		GL_RGB,
		GL_FLOAT,
		GL_COLOR_ATTACHMENT0
	};

	// Depth attachment
	Renderer::FBOAttachment depth = {};
	{
		depth.intFormat = GL_DEPTH_COMPONENT16;
		depth.slot      = GL_DEPTH_ATTACHMENT;
	}

	// Draw buffers
	std::vector<GLenum> drawBuffers =
	{
		color0.slot
	};

	// Set FBO dimensions
	FBO->width  = dimensions.x;
	FBO->height = dimensions.y;

	// Create FBO
	FBO->CreateFrameBuffer();
	FBO->Bind();
	FBO->AddTextureCubeMap(FBO->colorTextures[0], color0);
	FBO->AddBuffer(FBO->depthRenderBuffer, depth);
	FBO->SetDrawBuffers(drawBuffers);
	FBO->CheckStatus();
	FBO->EnableDepth();
	FBO->Unbind();

	// Return
	return FBO;
}

FbPtr IBLRenderer::Create2DFBO(const glm::ivec2& dimensions)
{
	// Allocate FBO on CPU
	auto FBO = std::make_shared<FrameBuffer>();

	// Color attachment
	Renderer::FBOAttachment color0 =
	{
		GL_LINEAR,
		GL_LINEAR,
		GL_CLAMP_TO_EDGE,
		GL_RG16F,
		GL_RG,
		GL_FLOAT,
		GL_COLOR_ATTACHMENT0
	};

	// Draw buffers
	std::vector<GLenum> drawBuffers =
	{
		color0.slot
	};

	// Set FBO dimensions
	FBO->width  = dimensions.x;
	FBO->height = dimensions.y;

	// Create FBO
	FBO->CreateFrameBuffer();
	FBO->Bind();
	FBO->AddTexture(FBO->colorTextures[0], color0);
	FBO->SetDrawBuffers(drawBuffers);
	FBO->CheckStatus();
	FBO->Unbind();

	// Return
	return FBO;
}

glm::mat4 IBLRenderer::LoadProjection()
{
	// Projection matrix
	return glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
}

std::array<glm::mat4, 6> IBLRenderer::LoadViews()
{
	// View matrix array
	return
	{
		glm::lookAt(glm::vec3(0.0f), glm::vec3( 1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
		glm::lookAt(glm::vec3(0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
		glm::lookAt(glm::vec3(0.0f), glm::vec3( 0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
		glm::lookAt(glm::vec3(0.0f), glm::vec3( 0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
		glm::lookAt(glm::vec3(0.0f), glm::vec3( 0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
		glm::lookAt(glm::vec3(0.0f), glm::vec3( 0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
	};
}

VAO IBLRenderer::LoadCube()
{
	// Cube vertices
	const std::vector<f32> vertices =
	{
		// Face #1
		-1.0f, -1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		// Face #2
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,
		// Face #3
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		// Face #4
		1.0f,  1.0f,  1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f, -1.0f,  1.0f,
		// Face #5
		-1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		// Face #6
		-1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f , 1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
	};

	// Return cube
	return std::make_shared<VertexArray>(3, vertices);
}

VAO IBLRenderer::LoadQuad()
{
	// Quad vertices
	const std::vector<f32> QUAD_VERTICES =
	{
		-1.0f,  1.0f,
		-1.0f, -1.0f,
	 	 1.0f,  1.0f,
		 1.0f, -1.0f
	};

	// Return quad
	return std::make_shared<VertexArray>(2, QUAD_VERTICES);
}