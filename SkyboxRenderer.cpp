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
	rotation += SKYBOX_ROTATION_SPEED * g_Delta;
	glm::mat4 matrix = Maths::CreateModelMatrixR
	(
		glm::vec3(0.0f, rotation, 0.0f)
	);
	shader.LoadModelMatrix(matrix);
}

void SkyboxRenderer::UnbindSkybox()
{
	glBindVertexArray(0);
}