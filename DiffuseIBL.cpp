#include "DiffuseIBL.h"

#include <array>

#include "GLM.h"
#include "ConverterShader.h"

using namespace Renderer;

using VAO   = DiffuseIBL::VAO;
using TxPtr = DiffuseIBL::TxPtr;
using FbPtr = DiffuseIBL::FbPtr;

constexpr glm::ivec2 CUBEMAP_DIMENSIONS = {512, 512};

DiffuseIBL::DiffuseIBL()
	: cubeMap(std::make_shared<FrameBuffer>()),
	  irradiance(std::make_shared<FrameBuffer>())
{
	ConvertToCubeMap();
}

void DiffuseIBL::ConvertToCubeMap()
{
	Renderer::FBOAttachment color0 =
	{
		GL_LINEAR,
		GL_LINEAR,
		GL_CLAMP_TO_EDGE,
	GL_RGB16F,
		GL_RGB,
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

	cubeMap->width  = CUBEMAP_DIMENSIONS.x;
	cubeMap->height = CUBEMAP_DIMENSIONS.y;

	cubeMap->CreateFrameBuffer();
	cubeMap->Bind();
	cubeMap->AddTextureCubeMap(cubeMap->colorTextures[0], color0);
	cubeMap->AddBuffer(cubeMap->depthRenderBuffer, depth);
	cubeMap->SetDrawBuffers(drawBuffers);
	cubeMap->CheckStatus();
	cubeMap->EnableDepth();
	cubeMap->Unbind();

	glm::mat4 projection = glm::perspective
	(
		glm::radians(90.0f),
		1.0f,
		0.1f,
		10.0f
	);

	std::array<glm::mat4, 6> views =
	{
		glm::lookAt(glm::vec3(0.0f), glm::vec3( 1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
		glm::lookAt(glm::vec3(0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
		glm::lookAt(glm::vec3(0.0f), glm::vec3( 0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
		glm::lookAt(glm::vec3(0.0f), glm::vec3( 0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
		glm::lookAt(glm::vec3(0.0f), glm::vec3( 0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
		glm::lookAt(glm::vec3(0.0f), glm::vec3( 0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
	};

	// Data
	TxPtr hdrMap = LoadHDRMap();
	VAO   cube   = LoadCube();
	// Shaders and renderers
	auto shader = Shader::ConverterShader();

	// Bind
	cubeMap->Bind();
	// Use GL_LEQUAL to avoid Z fighting
	glDepthFunc(GL_LEQUAL);
	// Disable depth writing for performance
	glDepthMask(GL_FALSE);
	// Load projection
	shader.LoadProjection(projection);
	// Bind HDR map
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, hdrMap->id);

	// For all views of the cube map
	for (usize i = 0; i < 6; ++i)
	{
		// Load view matrix
		shader.LoadView(views[i]);
		// Assign texture
		glFramebufferTexture2D
		(
			GL_FRAMEBUFFER,
			GL_COLOR_ATTACHMENT0,
			GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
			cubeMap->id,
			0
		);
		// Render cube
		glDrawArrays(GL_TRIANGLES, 0, cube->vertexCount);
	}

	// Reset
	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LESS);
	// Unbind
	cubeMap->Unbind();
}

TxPtr DiffuseIBL::LoadHDRMap()
{
	TxPtr hdrMap = std::make_shared<Texture>();

	auto data = hdrMap->LoadImageHDR("gfx/envMap.hdr");

	hdrMap->CreateTexture();
	hdrMap->Bind();

	hdrMap->SetParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	hdrMap->SetParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	hdrMap->SetParameter(GL_TEXTURE_WRAP_S,     GL_REPEAT);
	hdrMap->SetParameter(GL_TEXTURE_WRAP_T,     GL_REPEAT);
	hdrMap->SetParameter(GL_TEXTURE_LOD_BIAS,   TEXTURE_LOD_BIAS);

	hdrMap->SetPixelParameter(GL_UNPACK_ALIGNMENT, 1);
	hdrMap->LoadImageData(reinterpret_cast<u8*>(data), GL_RGB, GL_RGB, GL_FLOAT);
	hdrMap->GenerateMipmaps();

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