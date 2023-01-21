#ifndef POST_PROCESS_SHADER_H
#define POST_PROCESS_SHADER_H

#include "Shader.h"
#include "Util.h"

namespace Shader
{
	class PostProcessShader : public ShaderProgram
	{
	public:
		// Constructor
		PostProcessShader();
		// Connect texture units
		void ConnectTextureUnits();
		// Load bloom strength
		void LoadBloomStrength(f32 strength);
		// Get all uniform locations
		void GetUniformLocations() override;
	};
}

#endif
