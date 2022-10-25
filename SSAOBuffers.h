#ifndef SSAO_BUFFER_H
#define SSAO_BUFFER_H

#include <memory>
#include <vector>

#include "FrameBuffer.h"
#include "GLM.h"
#include "Util.h"
#include "Texture.h"

namespace Renderer
{
	class SSAOBuffers
	{
	public:
		using FbPtr = std::shared_ptr<FrameBuffer>;
		using TxPtr = std::shared_ptr<Texture>;
		using Vec3s = std::vector<glm::vec3>;

		SSAOBuffers();

		void BindSSAOBuffer() const;
		void BindBlurBuffer() const;
		void BindDefaultFBO() const;

		FbPtr ssaoBuffer;
		FbPtr blurBuffer;
		TxPtr noiseBuffer;
	private:
		Vec3s GetSampleKernel(usize size);
		Vec3s GetNoiseKernel(usize size);
	};
}

#endif
