#include "SkyboxRenderer.h"

#include <GL/glew.h>

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
	// Prepare skybox
	PrepareSkybox(skybox);
	// Draw skybox
	glDrawArrays(GL_TRIANGLES, 0, skybox.vao->vertexCount);
	// Unbind skybox
	UnbindSkybox();
}

void SkyboxRenderer::PrepareSkybox(const Entities::Skybox& skybox)
{
	// Bind VAO
	glBindVertexArray(skybox.vao->id);
	// Bind cube map
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, skybox.cubeMap->id);
}

void SkyboxRenderer::UnbindSkybox()
{
	// Unbind VAO
	glBindVertexArray(0);
}