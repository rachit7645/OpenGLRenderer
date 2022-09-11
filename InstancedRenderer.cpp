#include "InstancedRenderer.h"

using namespace Renderer;

InstancedRenderer::InstancedRenderer(Shader::InstancedShader& shader)
	: shader(shader),
	  buffer(std::make_shared<InstanceBuffer>())
{
	shader.Start();
	shader.ConnectTextureUnits();
	shader.Stop();
}

void InstancedRenderer::Render(const Batch& batch)
{
	for (const auto& [model, entities] : batch)
	{
		buffer->LoadInstanceData(entities);
		for (const auto& mesh : model->meshes)
		{
			glBindVertexArray(mesh.vao->id);

			const MeshTextures& textures = mesh.textures;
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, textures.diffuse->id);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, textures.specular->id);

			glDrawElementsInstanced
			(
				GL_TRIANGLES,
				mesh.vao->vertexCount,
				GL_UNSIGNED_INT,
				nullptr,
				static_cast<GLint>(entities.size())
			);

			glBindVertexArray(0);
		}
	}
}
