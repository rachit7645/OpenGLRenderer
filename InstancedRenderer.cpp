#include "InstancedRenderer.h"

using namespace Renderer;

using Shader::InstancedShader;
using Shader::FastInstancedShader;

InstancedRenderer::InstancedRenderer(InstancedShader& shader, FastInstancedShader& fastShader)
	: shader(shader),
	  fastShader(fastShader),
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

	// Bind diffuse
	const MeshTextures& textures = mesh.textures;
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textures.diffuse->id);

	if (mode == Mode::Normal)
	{
		// Bind specular
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, textures.specular->id);
		// Load materials
		shader.LoadMaterial(mesh.material);
	}
}

void InstancedRenderer::UnbindMesh()
{
	glBindVertexArray(0);
}
