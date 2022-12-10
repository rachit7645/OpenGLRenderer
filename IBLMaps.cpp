#include "IBLMaps.h"

#include <array>
#include <vector>

#include "ConverterShader.h"
#include "ConvolutionShader.h"
#include "RenderConstants.h"
#include "PreFilterShader.h"
#include "BRDFShader.h"
#include "Settings.h"

// Using namespaces
using namespace Renderer;

// Usings
using Engine::Settings;

// Aliases
using VAO   = IBLMaps::VAO;
using TxPtr = IBLMaps::TxPtr;
using FbPtr = IBLMaps::FbPtr;

// Constants
constexpr usize      PRE_FILTER_MIPMAP_LEVELS = {5};
constexpr glm::ivec2 CUBEMAP_DIMENSIONS       = {1024, 1024};
constexpr glm::ivec2 IRRADIANCE_DIMENSIONS    = {32,     32};
constexpr glm::ivec2 PRE_FILTER_DIMENSIONS    = {128,   128};
constexpr glm::ivec2 BRDF_LUT_DIMENSIONS      = {512,   512};

IBLMaps::IBLMaps()
{
	// Projection
	const glm::mat4 projection = glm::perspective
	(
		glm::radians(90.0f),
		1.0f,
		0.1f,
		10.0f
	);

	// Views (per-face)
	const std::array<glm::mat4, 6> views =
	{
		glm::lookAt(glm::vec3(0.0f), glm::vec3( 1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
		glm::lookAt(glm::vec3(0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
		glm::lookAt(glm::vec3(0.0f), glm::vec3( 0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
		glm::lookAt(glm::vec3(0.0f), glm::vec3( 0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
		glm::lookAt(glm::vec3(0.0f), glm::vec3( 0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
		glm::lookAt(glm::vec3(0.0f), glm::vec3( 0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
	};

	// Cube
	const VAO cube = LoadCube();

	// Convert equiangular map to a cube map
	ConvertToCubeMap(projection, views, cube);
	// Create the diffuse irradiance map
	GenerateIrradiance(projection, views, cube);
	// Create the pre=filtered specular map
	PreFilterSpecular(projection, views, cube);
	// Calculate BRDF lookup texture
	CalculateBRDF();
}

void IBLMaps::ConvertToCubeMap
(
	const glm::mat4& projection,
	const std::array<glm::mat4, 6>& views,
	const VAO& cube
)
{
	// Create cube map FBO
	auto cubeMapFBO = CreateCubeMapFBO(CUBEMAP_DIMENSIONS, true);

	// Data
	TxPtr hdrMap = LoadHDRMap();
	// Shaders and renderers
	auto shader = Shader::ConverterShader();
	shader.DumpToFile("dumps/CNV.s");

	// Prepare
	PrepareRender(cubeMapFBO, cube);
	// Start shader
	shader.Start();
	shader.ConnectTextureUnits();
	// Load projection
	shader.LoadProjection(projection);
	// Bind HDR map
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, hdrMap->id);

	// For each face
	for (usize i = 0; i < 6; ++i)
	{
		shader.LoadView(views[i]);
		RenderCubeFace(cubeMapFBO, cube, i);
	}

	// Unbind
	UnbindRender(cubeMapFBO);
	shader.Stop();

	cubeMap = cubeMapFBO->colorTextures[0];

	// Generate mipmaps
	cubeMap->Bind();
	cubeMap->GenerateMipmaps();
	cubeMap->Unbind();
}

void IBLMaps::GenerateIrradiance
(
	const glm::mat4& projection,
	const std::array<glm::mat4, 6>& views,
	const VAO& cube
)
{
	// Create cube map FBO
	auto irradianceFBO = CreateCubeMapFBO(IRRADIANCE_DIMENSIONS);

	// Shaders and renderers
	auto shader = Shader::ConvolutionShader();
	shader.DumpToFile("dumps/CNU.s");

	// Prepare
	PrepareRender(irradianceFBO, cube);
	// Start shader
	shader.Start();
	shader.ConnectTextureUnits();
	// Load projection
	shader.LoadProjection(projection);
	// Bind HDR map
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMap->id);

	// For each face
	for (usize i = 0; i < 6; ++i)
	{
		shader.LoadView(views[i]);
		RenderCubeFace(irradianceFBO, cube, i);
	}

	// Unbind
	UnbindRender(irradianceFBO);
	shader.Stop();

	irradiance = irradianceFBO->colorTextures[0];
}

void IBLMaps::PreFilterSpecular
(
	const glm::mat4& projection,
	const std::array<glm::mat4, 6>& views,
	const VAO& cube
)
{
	// Create cube map FBO
	auto preFilterFBO = CreateCubeMapFBO(PRE_FILTER_DIMENSIONS, true);

	// Generate mipmaps
	preFilterFBO->colorTextures[0]->Bind();
	preFilterFBO->colorTextures[0]->GenerateMipmaps();
	preFilterFBO->colorTextures[0]->Unbind();

	// Shaders and renderers
	auto shader = Shader::PreFilterShader();
	shader.DumpToFile("dumps/PFS.s");

	// Prepare
	PrepareRender(preFilterFBO, cube);
	// Start shader
	shader.Start();
	shader.ConnectTextureUnits();
	// Load projection
	shader.LoadProjection(projection);
	// Load resolution
	shader.LoadResolution(CUBEMAP_DIMENSIONS);
	// Bind HDR map
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
		shader.LoadRoughness(roughness);

		// For each face
		for (usize i = 0; i < 6; ++i)
		{
			shader.LoadView(views[i]);
			RenderCubeFace(preFilterFBO, cube, i, static_cast<GLint>(mip));
		}
	}

	// Unbind
	UnbindRender(preFilterFBO);
	shader.Stop();

	preFilter = preFilterFBO->colorTextures[0];
}

void IBLMaps::CalculateBRDF()
{
	// Create 2D FBO
	auto brdfFBO = Create2DFBO(BRDF_LUT_DIMENSIONS);
	auto quad    = LoadQuad();

	// Shaders and renderers
	auto shader = Shader::BRDFShader();
	shader.DumpToFile("dumps/BRDF.s");

	// Prepare
	PrepareRender(brdfFBO, quad);
	// Start shader
	shader.Start();

	// Draw
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, quad->vertexCount);

	// Unbind
	UnbindRender(brdfFBO);
	shader.Stop();

	brdfLut = brdfFBO->colorTextures[0];
}

FbPtr IBLMaps::CreateCubeMapFBO(const glm::ivec2& dimensions, bool isMipMapped)
{
	auto FBO = std::make_shared<FrameBuffer>();

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

	Renderer::FBOAttachment depth = {};
	{
		depth.intFormat = GL_DEPTH_COMPONENT24;
		depth.slot      = GL_DEPTH_ATTACHMENT;
	}

	std::vector<GLenum> drawBuffers =
	{
		color0.slot
	};

	FBO->width  = dimensions.x;
	FBO->height = dimensions.y;

	FBO->CreateFrameBuffer();
	FBO->Bind();
	FBO->AddTextureCubeMap(FBO->colorTextures[0], color0);
	FBO->AddBuffer(FBO->depthRenderBuffer, depth);
	FBO->SetDrawBuffers(drawBuffers);
	FBO->CheckStatus();
	FBO->EnableDepth();
	FBO->Unbind();

	return FBO;
}

FbPtr IBLMaps::Create2DFBO(const glm::ivec2& dimensions)
{
	auto FBO = std::make_shared<FrameBuffer>();

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

	Renderer::FBOAttachment depth = {};
	{
		depth.intFormat = GL_DEPTH_COMPONENT24;
		depth.slot      = GL_DEPTH_ATTACHMENT;
	}

	std::vector<GLenum> drawBuffers =
	{
		color0.slot
	};

	FBO->width  = dimensions.x;
	FBO->height = dimensions.y;

	FBO->CreateFrameBuffer();
	FBO->Bind();
	FBO->AddTexture(FBO->colorTextures[0], color0);
	FBO->AddBuffer(FBO->depthRenderBuffer, depth);
	FBO->SetDrawBuffers(drawBuffers);
	FBO->CheckStatus();
	FBO->EnableDepth();
	FBO->Unbind();

	return FBO;
}

void IBLMaps::PrepareRender(FbPtr& FBO, const VAO& cube)
{
	// Bind
	FBO->Bind();
	// Set rasterizer state
	glDepthFunc(GL_LEQUAL);
	glDisable(GL_CULL_FACE);
	// Bind vao
	glBindVertexArray(cube->id);
}

void IBLMaps::RenderCubeFace(FbPtr& FBO, const VAO& cube, usize face, GLint level)
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
	glDrawArrays(GL_TRIANGLES, 0, cube->vertexCount);
}

void IBLMaps::UnbindRender(FbPtr& FBO)
{
	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);
	glBindVertexArray(0);
	FBO->Unbind();
}

TxPtr IBLMaps::LoadHDRMap()
{
	// Get settings
	const auto& settings = Settings::GetInstance();
	// Load HDR map
	TxPtr hdrMap = std::make_shared<Texture>();

	stbi_set_flip_vertically_on_load(true);
	auto data = hdrMap->LoadImageHDR(settings.ibl.envMapPath);
	stbi_set_flip_vertically_on_load(false);

	hdrMap->CreateTexture();
	hdrMap->Bind();

	hdrMap->SetParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	hdrMap->SetParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	hdrMap->SetParameter(GL_TEXTURE_WRAP_S,     GL_REPEAT);
	hdrMap->SetParameter(GL_TEXTURE_WRAP_T,     GL_REPEAT);
	hdrMap->SetParameter(GL_TEXTURE_LOD_BIAS,   TEXTURE_LOD_BIAS);

	hdrMap->SetPixelParameter(GL_UNPACK_ALIGNMENT, 1);
	hdrMap->LoadImageData(reinterpret_cast<u8*>(data), GL_RGB16F, GL_RGB, GL_FLOAT);

	hdrMap->Unbind();
	stbi_image_free(data);

	return hdrMap;
}

VAO IBLMaps::LoadCube()
{
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

	return std::make_shared<VertexArray>(3, vertices);
}

VAO IBLMaps::LoadQuad()
{
	const std::vector<f32> QUAD_VERTICES =
	{
		-1.0f,  1.0f,
		-1.0f, -1.0f,
		 1.0f,  1.0f,
		 1.0f, -1.0f
	};

	return std::make_shared<VertexArray>(2, QUAD_VERTICES);
}