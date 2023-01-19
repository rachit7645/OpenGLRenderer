#include "MeshTextures.h"

using namespace Renderer;


MeshTextures::MeshTextures
(
	TxPtr albedo,
	TxPtr normal,
	TxPtr aoRghMtl,
	TxPtr emmisive
)
	: albedo(std::move(albedo)),
	  normal(std::move(normal)),
	  aoRghMtl(std::move(aoRghMtl)),
	  emmisive(std::move(emmisive))
{
}