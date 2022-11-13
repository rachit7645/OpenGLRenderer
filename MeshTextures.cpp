#include "MeshTextures.h"

using namespace Renderer;


MeshTextures::MeshTextures
(
	TxPtr albedo,
	TxPtr normal,
	TxPtr aoMtlRgh
)
	: albedo(std::move(albedo)),
	  normal(std::move(normal)),
	  aoMtlRgh(std::move(aoMtlRgh))
{
}