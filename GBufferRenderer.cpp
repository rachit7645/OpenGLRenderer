#include "GBufferRenderer.h"

#include <utility>

using namespace Renderer;

using Shader::GBufferShader;

GBufferRenderer::GBufferRenderer(GBufferShader& shader, BufferPtr instances)
	: shader(shader),
	  instances(std::move(instances))
{
	shader.Start();
	shader.ConnectTextureUnits();
	shader.Stop();
}

void GBufferRenderer::Render(const Batch& batch)
{
	BeginRender();
	for (const auto& [model, entities] : batch)
	{
		LoadData(entities);
		for (const auto& mesh : model->meshes)
		{
			PrepareMesh(mesh);
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
	EndRender();
}

void GBufferRenderer::BeginRender()
{
	instances->Bind();
	shader.Start();
}

void GBufferRenderer::EndRender()
{
	instances->Unbind();
	shader.Stop();
}

void GBufferRenderer::LoadData(const EntityVector& entities)
{
	instances->LoadInstanceData(entities);
}

void GBufferRenderer::PrepareMesh(const Mesh& mesh)
{
	glBindVertexArray(mesh.vao->id);
	LoadDiffuse(mesh);
	LoadSpecular(mesh);
}

void GBufferRenderer::LoadDiffuse(const Mesh& mesh)
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh.textures.diffuse->id);
}

void GBufferRenderer::LoadSpecular(const Mesh& mesh)
{
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, mesh.textures.specular->id);
	shader.LoadMaterial(mesh.material);
}

void GBufferRenderer::UnbindMesh()
{
	glBindVertexArray(0);
}