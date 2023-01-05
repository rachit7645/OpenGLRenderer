#ifndef POST_PROCESS_SHADER_H
#define POST_PROCESS_SHADER_H

#include "Shader.h"

namespace Shader
{
	class PostProcessShader : public ShaderProgram
	{
	public:
		// Constructor
		PostProcessShader();
		// Connect texture units
		void ConnectTextureUnits();
		// Get all uniform locations
		void GetUniformLocations() override;
	};
}

#endif
