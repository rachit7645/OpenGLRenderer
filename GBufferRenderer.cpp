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
	// Begin render pass
	BeginRender();
	// For each pair
	for (const auto& [model, entities] : batch)
	{
		// Load instance data
		LoadData(entities);
		// For each mesh
		for (const auto& mesh : model->meshes)
		{
			// Prepare mesh
			PrepareMesh(mesh);
			// Draw instances
			glDrawElementsInstanced
			(
				GL_TRIANGLES,
				mesh.vao->vertexCount,
				GL_UNSIGNED_INT,
				nullptr,
				static_cast<GLint>(entities.size())
			);
			// Unbind mesh
			UnbindMesh();
		}
	}
	// End render pass
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
	// Activate ambient occlusion, metallic and roughness
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, mesh.textures.aoRghMtl->id);
	// Activate emmisive
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, mesh.textures.emmisive->id);
}

void GBufferRenderer::UnbindMesh()
{
	glBindVertexArray(0);
}