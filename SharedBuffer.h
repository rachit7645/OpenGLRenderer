#ifndef SHARED_BUFFER_H
#define SHARED_BUFFER_H

#include "GLM.h"
#include "UniformBuffer.h"
#include "Camera.h"

namespace Renderer
{
	class SharedBuffer : public UniformBuffer
	{
	public:
		SharedBuffer();

		void LoadClipPlane(const glm::vec4& clipPlane);
		void LoadSkyColor(const glm::vec4& skyColor);
		void LoadCameraPos(const Entities::Camera& camera);
		void LoadFrustum(const glm::vec2& frustum);
	};

	namespace Detail
	{
		struct SharedBufferGLSL
		{
			glm::vec4 clipPlane;
			glm::vec4 skyColor;
			glm::vec4 cameraPos;
			glm::vec2 frustum;
		};
	}
}

#endif
