#include "DiffuseIBL.h"

#include <array>

#include "ConverterShader.h"
#include "ConvolutionShader.h"
#include "RenderConstants.h"

using namespace Renderer;

using VAO   = DiffuseIBL::VAO;
using TxPtr = DiffuseIBL::TxPtr;
using FbPtr = DiffuseIBL::FbPtr;

constexpr glm::ivec2 CUBEMAP_DIMENSIONS    = {512, 512};
constexpr glm::ivec2 IRRADIANCE_DIMENSIONS = {32, 32};

DiffuseIBL::DiffuseIBL()
	: cubeMap(std::make_shared<FrameBuffer>()),
	  irradiance(std::make_shared<FrameBuffer>())
{
	const glm::mat4 projection = glm::perspective
	(
		glm::radians(90.0f),
		1.0f,
		0.1f,
		10.0f
	);

	const std::array<glm::mat4, 6> views =
	{
		glm::lookAt(glm::vec3(0.0f), glm::vec3( 1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
		glm::lookAt(glm::vec3(0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
		glm::lookAt(glm::vec3(0.0f), glm::vec3( 0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
		glm::lookAt(glm::vec3(0.0f), glm::vec3( 0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
		glm::lookAt(glm::vec3(0.0f), glm::vec3( 0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
		glm::lookAt(glm::vec3(0.0f), glm::vec3( 0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
	};

	const VAO cube = LoadCube();

	ConvertToCubeMap(projection, views, cube);
	GenerateIrradiance(projection, views, cube);
}

TxPtr DiffuseIBL::GetCubeMap()
{
	return cubeMap->colorTextures[0];
}

TxPtr DiffuseIBL::GetIrradiance()
{
	return irradiance->colorTextures[0];
}

void DiffuseIBL::ConvertToCubeMap
(
	const glm::mat4& projection,
	const std::array<glm::mat4, 6>& views,
	const VAO& cube
)
{
	CreateCubeMapFBO(cubeMap, CUBEMAP_DIMENSIONS);

	// Data
	TxPtr hdrMap = LoadHDRMap();
	// Shaders and renderers
	auto shader = Shader::ConverterShader();

	// Prepare
	PrepareRender(cubeMap, cube);
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
		RenderCubeFace(cubeMap, cube, i);
	}

	// Unbind
	UnbindRender(cubeMap);
	shader.Stop();
}

void DiffuseIBL::GenerateIrradiance
(
	const glm::mat4& projection,
	const std::array<glm::mat4, 6>& views,
	const VAO& cube
)
{
	CreateCubeMapFBO(irradiance, IRRADIANCE_DIMENSIONS);

	// Shaders and renderers
	auto shader = Shader::ConvolutionShader();

	// Prepare
	PrepareRender(irradiance, cube);
	// Start shader
	shader.Start();
	shader.ConnectTextureUnits();
	// Load projection
	shader.LoadProjection(projection);
	// Bind HDR map
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMap->colorTextures[0]->id);

	// For each face
	for (usize i = 0; i < 6; ++i)
	{
		shader.LoadView(views[i]);
		RenderCubeFace(irradiance, cube, i);
	}

	// Unbind
	UnbindRender(irradiance);
	shader.Stop();
}

void DiffuseIBL::CreateCubeMapFBO(FbPtr& FBO, const glm::ivec2& dimensions)
{
	Renderer::FBOAttachment color0 =
	{
		GL_LINEAR,
		GL_LINEAR,
		GL_CLAMP_TO_EDGE,
		GL_RGBA16F,
		GL_RGBA,
		GL_FLOAT,
		GL_COLOR_ATTACHMENT0
	};

	Renderer::FBOAttachment depth = {};
	{
		depth.internalFormat = GL_DEPTH_COMPONENT24;
		depth.slot           = GL_DEPTH_ATTACHMENT;
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
}

void DiffuseIBL::PrepareRender(FbPtr& FBO, const VAO& cube)
{
	// Bind
	FBO->Bind();
	// Set rasterizer state
	glDepthFunc(GL_LEQUAL);
	glDisable(GL_CULL_FACE);
	// Bind vao
	glBindVertexArray(cube->id);
}

void DiffuseIBL::RenderCubeFace(FbPtr& FBO, const VAO& cube, usize face)
{
	// Assign texture
	glFramebufferTexture2D
	(
		GL_FRAMEBUFFER,
		GL_COLOR_ATTACHMENT0,
		GL_TEXTURE_CUBE_MAP_POSITIVE_X + face,
		FBO->colorTextures[0]->id,
		0
	);
	// Clear FBO
	glClearColor(GL_CLEAR_COLOR.r, GL_CLEAR_COLOR.g, GL_CLEAR_COLOR.b, GL_CLEAR_COLOR.a);
	glClear(GL_CLEAR_FLAGS);
	// Render cube
	glDrawArrays(GL_TRIANGLES, 0, cube->vertexCount);
}

void DiffuseIBL::UnbindRender(FbPtr& FBO)
{
	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);
	glBindVertexArray(0);
	FBO->Unbind();
}

TxPtr DiffuseIBL::LoadHDRMap()
{
	TxPtr hdrMap = std::make_shared<Texture>();

	stbi_set_flip_vertically_on_load(true);
	// TODO: Get a better map
	auto data = hdrMap->LoadImageHDR("gfx/newport_loft.hdr");
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

VAO DiffuseIBL::LoadCube()
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