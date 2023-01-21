#include "InstancedRenderer.h"

#include <utility>

// Using namespaces
using namespace Renderer;

// Usings
using Shader::FastInstancedShader;
using Shader::ShadowShader;

InstancedRenderer::InstancedRenderer
(
	FastInstancedShader& fastShader,
	ShadowShader& shadowShader,
	ShadowMap& shadowMap,
	IBLMaps& iblMaps,
	BufferPtr instances
)
	: fastShader(fastShader),
	  shadowShader(shadowShader),
	  shadowMap(shadowMap),
	  iblMaps(iblMaps),
	  instances(std::move(instances))
{
	// Connect texture units
	fastShader.Start();
	fastShader.ConnectTextureUnits();
	fastShader.Stop();
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
	// Bind instance SSBO
	instances->Bind();

	// Select mode
	switch (mode)
	{
	case Mode::Fast:
		// Start fast shader
		fastShader.Start();
		// Load IBL Textures
		LoadIBLTextures();
		break;

	case Mode::Shadow:
		// Start shadow shader
		shadowShader.Start();
		break;
	}
}

void InstancedRenderer::EndRender(Mode mode)
{
	// Unbind instance SSBO
	instances->Unbind();

	// Select mode
	switch (mode)
	{
	case Mode::Fast:
		// Stop fast shader
		fastShader.Stop();
		break;

	case Mode::Shadow:
		// Stop shadow shader
		shadowShader.Stop();
		break;
	}
}

void InstancedRenderer::LoadData(const EntityVector& entities)
{
	// Load data
	instances->LoadInstanceData(entities);
}

void InstancedRenderer::PrepareMesh(const Mesh& mesh, Mode mode)
{
	// Bind VAO
	glBindVertexArray(mesh.vao->id);

	// Select mode
	switch (mode)
	{
	case Mode::Fast:
		// Load textures
		LoadTextures(mesh);
		break;

	case Mode::Shadow:
		// We don't need anything here for shadows
		break;
	}
}

void InstancedRenderer::LoadTextures(const Mesh& mesh)
{
	// Load albedo
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh.textures.albedo->id);
	// Load aoMtlRgh
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, mesh.textures.aoRghMtl->id);
}

void InstancedRenderer::LoadIBLTextures()
{
	// Activate irradiance map
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_CUBE_MAP, iblMaps.irradiance->id);
	// Activate pre-filter map
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_CUBE_MAP, iblMaps.preFilter->id);
	// Activate BRDF LUT map
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, iblMaps.brdfLut->id);
}

void InstancedRenderer::UnbindMesh()
{
	glBindVertexArray(0);
}