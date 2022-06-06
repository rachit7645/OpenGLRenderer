#include "Image.h"
#include "stb/stb_image.h"

using namespace Util;

Image2D::Image2D(const std::string& path)
{
	data = stbi_load((Files::GetResourceDirectory() + path).c_str(), &width, &height, &channels, STBI_rgb);
}

Image2D::~Image2D()
{
	stbi_image_free(data);
}

u32 Image2D::GetARGB(int x, int y)
{
	if (x < 0 || x >= width || y < 0 || y >= height)
		return 0;

	ssize_t position = (y * width + x) * STBI_rgb;
	u8 red = data[position];
	u8 green = data[position + 1];
	u8 blue = data[position + 2];
	u8 alpha = 0;

	return alpha << 24 | red << 16 | green << 8 | blue;
}