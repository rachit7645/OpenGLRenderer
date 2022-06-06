#ifndef IMAGE_H
#define IMAGE_H

#include <string>

#include "Files.h"
#include "Util.h"

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

		// Returns ARGB value with alpha = 0, which is invalidated
		// Tries to emulate java's BufferedImage.getRGB() method
		u32 GetARGB(int x, int y);
	private:
		u8* data;
	};
}


#endif