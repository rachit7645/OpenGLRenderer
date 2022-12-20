#include "InstancedRenderer.h"

#include <utility>

using namespace Renderer;

using Shader::FastInstancedShader;
using Shader::ShadowInstancedShader;

InstancedRenderer::InstancedRenderer
(
	FastInstancedShader& fastShader,
	ShadowInstancedShader& shadowShader,
	ShadowMap& shadowMap,
	BufferPtr instances
)
	: fastShader(fastShader),
	  shadowShader(shadowShader),
	  shadowMap(shadowMap),
	  instances(std::move(instances))
{
}

void InstancedRenderer::Render(const Batch& batch, Mode mode)
{
	// Begin render pass
	BeginRender(mode);
	// For each pair
	for (const auto& [model, entities] : batch)
	{
		// Load instance data
		LoadData(entities);
		// For each mesh
		for (const auto& mesh : model->meshes)
		{
			// Prepare mesh
			PrepareMesh(mesh, mode);
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
	EndRender(mode);
}

void InstancedRenderer::BeginRender(Mode mode)
{
	instances->Bind();

	switch (mode)
	{
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
	instances->Unbind();

	switch (mode)
	{
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
	instances->LoadInstanceData(entities);
}

void InstancedRenderer::PrepareMesh(const Mesh& mesh, Mode mode)
{
	// Bind vao
	glBindVertexArray(mesh.vao->id);

	switch (mode)
	{
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
	glBindTexture(GL_TEXTURE_2D, mesh.textures.albedo->id);
}

void InstancedRenderer::UnbindMesh()
{
	glBindVertexArray(0);
}