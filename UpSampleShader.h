#ifndef UP_SAMPLE_SHADER_H
#define UP_SAMPLE_SHADER_H

#include "Shader.h"
#include "Util.h"

namespace Shader
{
	class UpSampleShader : public ShaderProgram
	{
	public:
		// Constructor
		UpSampleShader();
		// Connect texture units
		void ConnectTextureUnits();
		// Load filter radius
		void LoadFilterRadius(f32 radius);
		// Get all uniform locations
		void GetUniformLocations() override;
	};
}

#endif
