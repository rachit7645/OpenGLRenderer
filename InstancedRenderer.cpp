#include "InstancedRenderer.h"

using namespace Renderer;

InstancedRenderer::InstancedRenderer(Shader::InstancedShader& shader)
	: shader(shader),
	  m_buffer(std::make_shared<InstanceBuffer>())
{
	shader.Start();
	shader.ConnectTextureUnits();
	shader.Stop();
}

void InstancedRenderer::Render(const Batch& batch)
{
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
}

void InstancedRenderer::LoadData(const EntityVector& entities)
{
	m_buffer->LoadInstanceData(entities);
}

void InstancedRenderer::PrepareMesh(const Mesh& mesh)
{
	// Bind vao
	glBindVertexArray(mesh.vao->id);
	// Bind textures
	const MeshTextures& textures = mesh.textures;
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textures.diffuse->id);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, textures.specular->id);
}

void InstancedRenderer::UnbindMesh()
{
	glBindVertexArray(0);
}
