#ifndef SKYBOX_RENDERER_H
#define SKYBOX_RENDERER_H

#include <string>
#include <GL/glew.h>

#include "GLM.h"
#include "Util.h"
#include "SkyboxShader.h"
#include "Skybox.h"

namespace Renderer
{
	class SkyboxRenderer
	{
	public:
		SkyboxRenderer(Shader::SkyboxShader& shaderRef);
		Shader::SkyboxShader& shader;

		void Render(const Entities::Skybox& skybox);
	private:
		void PrepareSkybox(const Entities::Skybox& skybox);
		void UnbindSkybox();
	};
}

#endif