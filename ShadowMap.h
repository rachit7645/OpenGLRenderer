#ifndef SHADOW_MAP_H
#define SHADOW_MAP_H

#include <memory>

#include "GLM.h"
#include "FrameBuffer.h"
#include "Camera.h"
#include "ShadowMatrixBuffer.h"

namespace Renderer
{
	class ShadowMap
	{
	public:
		using FbPtr  = std::shared_ptr<FrameBuffer>;
		using MtxPtr = std::shared_ptr<ShadowMatrixBuffer>;
		using Vec4s  = std::vector<glm::vec4>;
		using Mat4s  = std::vector<glm::mat4>;

		ShadowMap();

		void Update(const Entities::Camera& camera, const glm::vec3& lightPos);

		void BindShadowFBO()  const;
		void BindDefaultFBO() const;

		FbPtr buffer;
	private:
		MtxPtr m_matrixBuffer;

		Mat4s CalculateLightSpaceMatrices
		(
			const Entities::Camera& camera,
			const glm::vec3& lightDir
		);

		glm::mat4 CalculateLightSpaceMatrix
		(
			f32 nearPlane,
			f32 farPlane,
			const Entities::Camera& camera,
			const glm::vec3& lightDir
		);

		Vec4s CalculateFrustumCorners(const glm::mat4& proj, const glm::mat4& view);
	};
}

#endif