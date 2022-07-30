#ifndef SKYBOX_RENDERER_H
#define SKYBOX_RENDERER_H

#include "Util.h"
#include "SkyboxShader.h"
#include "Skybox.h"

namespace Renderer
{
	constexpr auto SKYBOX_ROTATION_SPEED = 3.0f;

	class SkyboxRenderer
	{
	public:
		explicit SkyboxRenderer(Shader::SkyboxShader& shaderRef);
		Shader::SkyboxShader& shader;

		void Render(const Entities::Skybox& skybox);
	private:
		void PrepareSkybox(const Entities::Skybox& skybox);
		void LoadModelMatrix();
		void UnbindSkybox();

		f32 rotation = 0.0f;
	};
}

#endif