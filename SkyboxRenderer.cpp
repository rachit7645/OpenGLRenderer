#include "SkyboxRenderer.h"

using namespace Renderer;

using Shader::SkyboxShader;
using Entities::Skybox;

SkyboxRenderer::SkyboxRenderer(SkyboxShader& shaderRef) : shader(shaderRef) {}

void SkyboxRenderer::PrepareSkybox(const Entities::Skybox& skybox)
{
	glBindVertexArray(skybox.vao->id);
	glEnableVertexAttribArray(0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, skybox.cubeMap->id);
}

void SkyboxRenderer::Render(const Skybox& skybox)
{
	PrepareSkybox(skybox);
	glDrawArrays(GL_TRIANGLES, 0, skybox.vao->vertexCount);
	UnbindSkybox();
}

void SkyboxRenderer::UnbindSkybox()
{
	glDisableVertexAttribArray(0);
	glBindVertexArray(0);
}