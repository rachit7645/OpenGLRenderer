#ifndef DIFFUSE_IBL_H
#define DIFFUSE_IBL_H

#include <memory>

#include "FrameBuffer.h"
#include "Texture.h"
#include "VertexArray.h"

namespace Renderer
{
	class DiffuseIBL
	{
	public:
		// Usings
		using VAO   = std::shared_ptr<VertexArray>;
		using TxPtr = std::shared_ptr<Texture>;
		using FbPtr = std::shared_ptr<FrameBuffer>;
		// Main constructor
		DiffuseIBL();
		// Data
		FbPtr cubeMap;
		FbPtr irradiance;
	private:
		void  ConvertToCubeMap();
		TxPtr LoadHDRMap();
		VAO   LoadCube();
	};
}

#endif
