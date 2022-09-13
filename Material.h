#ifndef MATERIAL_H
#define MATERIAL_H

#include "Util.h"

namespace Renderer
{
	class Material
	{
	public:
		// Default constructor
		Material() = default;
		// Specular constructor
		Material
		(
			f32 shineDamper,
			f32 reflectivity
		);

		f32 shineDamper  = 1.0f; // Shine Damper
		f32 reflectivity = 0.0f; // Reflectivity
	};
}

#endif