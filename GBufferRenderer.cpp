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
	LoadTextures(mesh);
}

void GBufferRenderer::LoadTextures(const Mesh& mesh)
{
	// Activate albedo
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh.textures.albedo->id);
	// Activate normal
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, mesh.textures.normal->id);
	// Activate metallic
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, mesh.textures.metallic->id);
	// Activate roughness
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, mesh.textures.roughness->id);
	// Activate ao
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, mesh.textures.ao->id);
}

void GBufferRenderer::UnbindMesh()
{
	glBindVertexArray(0);
}