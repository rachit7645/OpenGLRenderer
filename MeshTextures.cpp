#include "MeshTextures.h"

using namespace Renderer;


MeshTextures::MeshTextures
(
	TxPtr albedo,
	TxPtr normal,
	TxPtr aoRghMtl
)
	: albedo(std::move(albedo)),
	  normal(std::move(normal)),
	  aoRghMtl(std::move(aoRghMtl))
{
}