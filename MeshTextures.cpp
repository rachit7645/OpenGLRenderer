#include "MeshTextures.h"

using namespace Renderer;


MeshTextures::MeshTextures
(
	TxPtr albedo,
	TxPtr normal,
	TxPtr metallic,
	TxPtr roughness,
	TxPtr ao
)
	: albedo(std::move(albedo)),
	  normal(std::move(normal)),
	  metallic(std::move(metallic)),
	  roughness(std::move(roughness)),
	  ao(std::move(ao))
{
}
