#include "MeshTextures.h"

using namespace Renderer;

MeshTextures::MeshTextures(TxPtr diffuse, TxPtr specular)
	: diffuse(std::move(diffuse)),
	  specular(std::move(specular))
{
}
