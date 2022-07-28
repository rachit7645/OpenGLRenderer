#include "Image.h"

#include "stb/stb_image.h"

#include "Files.h"
#include "Log.h"

using namespace Util;

Image2D::Image2D(const std::string& path)
{
	LOG_INFO("Loading image: ", path, "\n");
	m_data = stbi_load
	(
		(Files::GetResourceDirectory() + path).c_str(),
		&width,
		&height,
		&channels,
		STBI_rgb
	);

	if (m_data == nullptr)
	{
		LOG_ERROR("Failed to load image: ", path);
	}
}

u8 Image2D::GetRed(int x, int y) const
{
	assert(x >= 0 || x < width || y >= 0 || y < height);

	return m_data[GetPosition(x, y)];
}

u8 Image2D::GetGreen(int x, int y) const
{
	assert(x >= 0 || x < width || y >= 0 || y < height);

	return m_data[GetPosition(x, y) + 1];
}

u8 Image2D::GetBlue(int x, int y) const
{
	assert(x >= 0 || x < width || y >= 0 || y < height);

	return m_data[GetPosition(x, y) + 2];
}

u32 Image2D::GetARGB(int x, int y) const
{
	assert(x >= 0 || x < width || y >= 0 || y < height);

	ssize_t position = GetPosition(x, y);
	
	u8 red   = m_data[position];
	u8 green = m_data[position + 1];
	u8 blue  = m_data[position + 2];
	u8 alpha = 0;

	return alpha << 24 | red << 16 | green << 8 | blue;
}

ssize_t Image2D::GetPosition(int x, int y) const
{
	return (y * width + x) * channels;
}

Image2D::~Image2D()
{
	stbi_image_free(m_data);
}