#ifndef DIFFUSE_IBL_H
#define DIFFUSE_IBL_H

#include <memory>

#include "FrameBuffer.h"
#include "Texture.h"
#include "VertexArray.h"
#include "GLM.h"

namespace Renderer
{
	class IBLMaps
	{
	public:
		// Usings
		using VAO   = std::shared_ptr<VertexArray>;
		using TxPtr = std::shared_ptr<Texture>;
		using FbPtr = std::shared_ptr<FrameBuffer>;
		// Main constructor
		IBLMaps();
		// Data
		TxPtr cubeMap;
		TxPtr irradiance;
		TxPtr preFilter;
	private:
		// Converts equiangular map to cube map
		void ConvertToCubeMap
		(
			const glm::mat4& projection,
			const std::array<glm::mat4, 6>& views,
			const VAO& cube
		);
		// Generates irradiance map
		void GenerateIrradiance
		(
			const glm::mat4& projection,
			const std::array<glm::mat4, 6>& views,
			const VAO& cube
		);
		// Pre-filters the specular of the hdr map
		void PreFilterSpecular
		(
			const glm::mat4& projection,
			const std::array<glm::mat4, 6>& views,
			const VAO& cube
		);
		// Shared render functions
		FbPtr CreateCubeMapFBO(const glm::ivec2& dimensions, bool isMipMapped = false);
		void  PrepareRender(FbPtr& FBO, const VAO& cube);
		void  RenderCubeFace(FbPtr& FBO, const VAO& cube, usize face, GLint level = 0);
		void  UnbindRender(FbPtr& FBO);
		// Helper functions
		TxPtr LoadHDRMap();
		VAO   LoadCube();
	};
}

#endif
