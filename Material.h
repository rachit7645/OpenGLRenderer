#ifndef MATERIAL_H
#define MATERIAL_H

#include "Util.h"

namespace Renderer
{
	class Material
	{
	public:
		Material(f32 shineDamper, f32 reflectivity, bool isTransparent, bool useFakeLighting);
		Material(f32 shineDamper, f32 reflectivity);
		Material(bool isTransparent, bool useFakeLighting);
		Material();

		f32 shineDamper = 1.0f;						// Shine Dampner
		f32 reflectivity = 0.0f;					// Reflectivity

		bool isTransparent = false;					// Is Transparent or not
		bool useFakeLighting = false;				// Use fake lighting or not
	};
}

#endif