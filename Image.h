#ifndef IMAGE_H
#define IMAGE_H

#include <string>

#include "Files.h"
#include "Util.h"
#include "Log.h"

namespace Util
{
	class Image2D
	{
	public:
		Image2D(const std::string& path);
		~Image2D();

		int width;
		int height;
		int channels;

		// Returns the red component at position (y * width + x) * STBI_rgb
		u8 GetRed(int x, int y) const;
		// Returns the green component at position ((y * width + x) * STBI_rgb) + 1
		u8 GetGreen(int x, int y) const;
		// Returns the blue component at position ((y * width + x) * STBI_rgb) + 2
		u8 GetBlue(int x, int y) const;
		// Returns ARGB value with alpha = 0
		u32 GetARGB(int x, int y) const;
	private:
		u8* data;

		bool DimensionCheck(int x, int y) const;
		ssize_t GetPosition(int x, int y) const;
	};
}


#endif