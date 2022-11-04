#include "MeshTextures.h"

using namespace Renderer;


MeshTextures::MeshTextures
(
	TxPtr albedo,
	TxPtr normal,
	TxPtr mtlRgh,
	TxPtr ao
)
	: albedo(std::move(albedo)),
	  normal(std::move(normal)),
	  mtlRgh(std::move(mtlRgh)),
	  ao(std::move(ao))
{
}
