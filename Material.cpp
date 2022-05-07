#include "Material.h"

using namespace Renderer;

Material::Material() {}

Material::Material(f32 shineDamper, f32 reflectivity) : shineDamper{ shineDamper }, reflectivity{ reflectivity } {}

Material::Material(bool isTransparent, bool useFakeLighting) : isTransparent{ isTransparent }, useFakeLighting{ useFakeLighting } {}

Material::Material(f32 shineDamper, f32 reflectivity, bool isTransparent, bool useFakeLighting)
	: shineDamper{ shineDamper }, reflectivity{ reflectivity },
	isTransparent{ isTransparent }, useFakeLighting{ useFakeLighting } {}