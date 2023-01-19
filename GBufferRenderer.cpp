#include "GBufferRenderer.h"

using namespace Renderer;

using Shader::GBufferShader;

GBufferRenderer::GBufferRenderer(GBufferShader& shader, BufferPtr instances)
	: shader(shader),
	  instances(std::move(instances))
{
	// Connect texture units
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
	// Bind
	instances->Bind();
	shader.Start();
}

void GBufferRenderer::EndRender()
{
	// Unbind
	instances->Unbind();
	shader.Stop();
}

void GBufferRenderer::LoadData(const EntityVector& entities)
{
	// Load data
	instances->LoadInstanceData(entities);
}

void GBufferRenderer::PrepareMesh(const Mesh& mesh)
{
	// Bind VAO
	glBindVertexArray(mesh.vao->id);
	// Load textures
	LoadTextures(mesh);
}

void GBufferRenderer::LoadTextures(const Mesh& mesh)
{
	// Activate albedo
	mesh.textures.albedo->Bind(0);
	// Activate normal
	mesh.textures.normal->Bind(1);
	// Activate ambient occlusion, metallic and roughness
	mesh.textures.aoRghMtl->Bind(2);
	// Activate emmisive
	mesh.textures.emmisive->Bind(3);
}

void GBufferRenderer::UnbindMesh()
{
	// Unbind VAO
	glBindVertexArray(0);
}