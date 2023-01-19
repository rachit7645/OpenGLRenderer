#include "SkyboxRenderer.h"

#include <GL/glew.h>

#include "GLM.h"

// Using namespaces
using namespace Renderer;

// Usings
using Shader::SkyboxShader;
using Entities::Skybox;

SkyboxRenderer::SkyboxRenderer(SkyboxShader& shaderRef)
	: shader(shaderRef)
{
}

void SkyboxRenderer::Render(const Skybox& skybox)
{
	// Prepare
	PrepareSkybox(skybox);
	// Load matrix
	LoadModelMatrix();
	// Draw skybox
	glDrawArrays(GL_TRIANGLES, 0, skybox.vao->vertexCount);
	// Unbind
	UnbindSkybox();
}

void SkyboxRenderer::PrepareSkybox(const Entities::Skybox& skybox)
{
	// Bind VAO
	glBindVertexArray(skybox.vao->id);
	// Activate cube map texture
	skybox.cubeMap->Bind(0);
}

void SkyboxRenderer::LoadModelMatrix()
{
	// Load identity matrix
	shader.LoadModelMatrix(glm::mat4(1.0f));
}

void SkyboxRenderer::UnbindSkybox()
{
	// Unbind VAO
	glBindVertexArray(0);
}