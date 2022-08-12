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
		// Transparency constructor
		Material
		(
			bool isTransparent,
			bool useFakeLighting
		);

		f32 shineDamper  = 1.0f; // Shine Damper
		f32 reflectivity = 0.0f; // Reflectivity

		bool isTransparent   = false; // Is Transparent or not
		bool useFakeLighting = false; // Use fake lighting or not
	};
}

#endif