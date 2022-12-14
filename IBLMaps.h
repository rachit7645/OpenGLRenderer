#ifndef DIFFUSE_IBL_H
#define DIFFUSE_IBL_H

#include <memory>

#include "Texture.h"
#include "IBLRenderer.h"

namespace Renderer
{
	class IBLMaps
	{
	public:
		// Usings
		using TxPtr = std::shared_ptr<Texture>;

		// Main constructor
		IBLMaps(IBLRenderer& iblRenderer);

		// Data
		TxPtr cubeMap;
		TxPtr irradiance;
		TxPtr preFilter;
		TxPtr brdfLut;
	private:
		// Helper functions
		TxPtr LoadHDRMap();
	};
}

#endif
