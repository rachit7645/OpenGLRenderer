#ifndef SKYBOX_RENDERER_H
#define SKYBOX_RENDERER_H

#include "Util.h"
#include "SkyboxShader.h"
#include "Skybox.h"

namespace Renderer
{
	class SkyboxRenderer
	{
	public:
		// Main constructor
		explicit SkyboxRenderer(Shader::SkyboxShader& shaderRef);
		// Shader
		Shader::SkyboxShader& shader;
		// Render skybox
		void Render(const Entities::Skybox& skybox);
	private:
		// Prepare skybox for render
		void PrepareSkybox(const Entities::Skybox& skybox);
		// Load identity matrix
		void LoadModelMatrix();
		// Unbind skybox
		void UnbindSkybox();
	};
}

#endif