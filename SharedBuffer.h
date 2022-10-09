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
	};

	namespace Detail
	{
		struct SharedBufferGLSL
		{
			alignas(16) glm::vec4 clipPlane;
			alignas(16) glm::vec4 skyColor;
			alignas(16) glm::vec4 cameraPos;
		};
	}
}

#endif
