#include "Material.h"

using namespace Renderer;

Material::Material(f32 shineDamper, f32 reflectivity)
	: shineDamper(shineDamper),
	  reflectivity(reflectivity)
{
}