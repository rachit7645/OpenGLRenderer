#ifndef SHADOW_MAP_H
#define SHADOW_MAP_H

#include <memory>

#include "GLM.h"
#include "FrameBuffer.h"
#include "Camera.h"
#include "ShadowBuffer.h"

namespace Renderer
{
	class ShadowMap
	{
	public:
		// Usings
		using FbPtr  = std::shared_ptr<FrameBuffer>;
		using MtxPtr = std::shared_ptr<ShadowBuffer>;
		using Vec4s  = std::vector<glm::vec4>;
		using Mat4s  = std::vector<glm::mat4>;
		// Main constructor
		ShadowMap();
		// Update once per frame
		void Update(const Entities::Camera& camera, const glm::vec3& lightPos);
		// Bind buffer
		void BindShadowFBO()  const;
		// Unbind buffer
		void BindDefaultFBO() const;
		// Shadow buffer
		FbPtr buffer;
	private:
		// Shadow matrix buffer
		MtxPtr m_matrixBuffer;
		// Calculates all cascade matrices
		Mat4s CalculateLightSpaceMatrices
		(
			const Entities::Camera& camera,
			const glm::vec3& lightDir
		);
		// Calculates cascade matrix
		glm::mat4 CalculateLightSpaceMatrix
		(
			f32 nearPlane,
			f32 farPlane,
			const Entities::Camera& camera,
			const glm::vec3& lightDir
		);
		// Calculates center of the view frustum
		glm::vec3 CalculateCenter(const Vec4s& corners);
		// Calculates the light view matrix
		glm::mat4 CalculateViewMatrix(const Vec4s& corners, const glm::vec3& lightDir);
		// Calculates the light projection matrix
		glm::mat4 CalculateProjMatrix(const Vec4s& corners, const glm::mat4& lightView);
		// Calculates all frustum corners
		Vec4s CalculateFrustumCorners(const glm::mat4& proj, const glm::mat4& view);
	};
}

#endif