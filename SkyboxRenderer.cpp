#include "SkyboxRenderer.h"

#include <GL/glew.h>

#include "GLM.h"
#include "Maths.h"

using namespace Renderer;

using Shader::SkyboxShader;
using Entities::Skybox;

SkyboxRenderer::SkyboxRenderer(SkyboxShader& shaderRef)
	: shader(shaderRef)
{
}

void SkyboxRenderer::Render(const Skybox& skybox)
{
	PrepareSkybox(skybox);
	LoadModelMatrix();
	glDrawArrays(GL_TRIANGLES, 0, skybox.vao->vertexCount);
	UnbindSkybox();
}

void SkyboxRenderer::PrepareSkybox(const Entities::Skybox& skybox)
{
	glBindVertexArray(skybox.vao->id);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, skybox.cubeMap->id);
}

void SkyboxRenderer::LoadModelMatrix()
{
	shader.LoadModelMatrix(glm::mat4(1.0f));
}

void SkyboxRenderer::UnbindSkybox()
{
	glBindVertexArray(0);
}