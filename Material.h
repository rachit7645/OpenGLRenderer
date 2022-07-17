#ifndef MATERIAL_H
#define MATERIAL_H

#include "Util.h"

namespace Renderer
{
	// FIXME: This class is a mess
	class Material
	{
	public:
		Material
		(
			f32 shineDamper = 1.0f,
			f32 reflectivity = 0.0f,
			bool isTransparent = false,
			bool useFakeLighting = false
		);

		Material
		(
			bool isTransparent,
			bool useFakeLighting
		);

		f32 shineDamper  = 1.0f;					// Shine Dampner
		f32 reflectivity = 0.0f;					// Reflectivity

		bool isTransparent   = false;				// Is Transparent or not
		bool useFakeLighting = false;				// Use fake lighting or not
	};
}

#endif