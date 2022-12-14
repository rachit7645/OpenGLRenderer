#ifndef IBL_RENDERER_H
#define IBL_RENDERER_H

#include <memory>
#include <array>

#include "Texture.h"
#include "GLM.h"
#include "VertexArray.h"
#include "FrameBuffer.h"
#include "ConverterShader.h"
#include "ConvolutionShader.h"
#include "PreFilterShader.h"
#include "BRDFShader.h"

namespace Renderer
{
	class IBLRenderer
	{
	public:
		// Usings
		using VAO   = std::shared_ptr<VertexArray>;
		using TxPtr = std::shared_ptr<Texture>;
		using FbPtr = std::shared_ptr<FrameBuffer>;

		// Main Constructor
		IBLRenderer
		(
			Shader::ConverterShader& converter,
			Shader::ConvolutionShader& convolution,
			Shader::PreFilterShader& preFilter,
			Shader::BRDFShader& brdf
		);

		// Shaders
		Shader::ConverterShader&   converter;
		Shader::ConvolutionShader& convolution;
		Shader::PreFilterShader&   preFilter;
		Shader::BRDFShader&        brdf;

		// Converts equiangular map to cube map
		void ConvertToCubeMap(TxPtr& cubeMap, TxPtr& hdrMap);
		// Generates irradiance map
		void GenerateIrradiance(TxPtr& irradiance, TxPtr& cubeMap);
		// Pre-filters the specular of the hdr map
		void PreFilterSpecular(TxPtr& preFilterMap, TxPtr& cubeMap);
		// Calculates BRDF LUT
		void CalculateBRDF(TxPtr& brdfLut);
	private:
		void PrepareRender(FbPtr& FBO, const VAO& vao);
		void UnbindRender(FbPtr& FBO);

		void RenderCubeFace(FbPtr& FBO, usize face, GLint level = 0);
		void RenderQuad();

		FbPtr CreateCubeMapFBO(const glm::ivec2& dimensions, bool isMipMapped = false);
		FbPtr Create2DFBO(const glm::ivec2& dimensions);

		glm::mat4 LoadProjection();
		std::array<glm::mat4, 6> LoadViews();

		VAO LoadCube();
		VAO LoadQuad();

		glm::mat4 m_projection;
		std::array<glm::mat4, 6> m_views;

		VAO m_cube;
		VAO m_quad;
	};
}

#endif
