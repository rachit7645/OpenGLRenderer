#include "InstancedRenderer.h"

using namespace Renderer;

using Shader::InstancedShader;
using Shader::FastInstancedShader;
using Shader::ShadowInstancedShader;

InstancedRenderer::InstancedRenderer
(
	InstancedShader& shader,
	FastInstancedShader& fastShader,
	ShadowInstancedShader& shadowShader,
	ShadowMap& shadowMap
)
	: shader(shader),
	  fastShader(fastShader),
	  shadowShader(shadowShader),
	  shadowMap(shadowMap),
	  m_buffer(std::make_shared<InstanceBuffer>())
{
	shader.Start();
	shader.ConnectTextureUnits();
	shader.Stop();
}

void InstancedRenderer::Render(const Batch& batch, Mode mode)
{
	BeginRender(mode);
	for (const auto& [model, entities] : batch)
	{
		LoadData(entities);
		for (const auto& mesh : model->meshes)
		{
			PrepareMesh(mesh, mode);
			glDrawElementsInstanced
			(
				GL_TRIANGLES,
				mesh.vao->vertexCount,
				GL_UNSIGNED_INT,
				nullptr,
				static_cast<GLint>(entities.size())
			);
			UnbindMesh();
		}
	}
	EndRender(mode);
}

void InstancedRenderer::BeginRender(Mode mode)
{
	m_buffer->Bind();

	switch (mode)
	{
	case Mode::Normal:
		shader.Start();
		break;

	case Mode::Fast:
		fastShader.Start();
		break;

	case Mode::Shadow:
		shadowShader.Start();
		break;
	}
}

void InstancedRenderer::EndRender(Mode mode)
{
	m_buffer->Unbind();

	switch (mode)
	{
	case Mode::Normal:
		shader.Stop();
		break;

	case Mode::Fast:
		fastShader.Stop();
		break;

	case Mode::Shadow:
		shadowShader.Stop();
		break;
	}
}

void InstancedRenderer::LoadData(const EntityVector& entities)
{
	m_buffer->LoadInstanceData(entities);
}

void InstancedRenderer::PrepareMesh(const Mesh& mesh, Mode mode)
{
	// Bind vao
	glBindVertexArray(mesh.vao->id);

	switch (mode)
	{
	case Mode::Normal:
		LoadDiffuse(mesh);
		LoadSpecular(mesh);
		LoadShadowMap();
		break;

	case Mode::Fast:
		LoadDiffuse(mesh);
		break;

	case Mode::Shadow:
		break;
	}
}

void InstancedRenderer::LoadDiffuse(const Mesh& mesh)
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh.textures.diffuse->id);
}

void InstancedRenderer::LoadSpecular(const Mesh& mesh)
{
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, mesh.textures.specular->id);
	shader.LoadMaterial(mesh.material);
}

void InstancedRenderer::LoadShadowMap()
{
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D_ARRAY, shadowMap.buffer->depthTexture->id);
}

void InstancedRenderer::UnbindMesh()
{
	glBindVertexArray(0);
}