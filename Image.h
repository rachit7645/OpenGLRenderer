#ifndef IMAGE_H
#define IMAGE_H

#include <string_view>

#include "Util.h"

namespace Util
{
	class Image2D
	{
	public:
		// Constructor
		explicit Image2D(const std::string_view path);
		// Destructor
		~Image2D();

		// Copy constructor
		Image2D(const Image2D&) = delete;
		// Move constructor
		Image2D(Image2D&&) = default;

		// Copy assignment
		Image2D& operator=(const Image2D&) = delete;
		// Move assignment
		Image2D& operator=(Image2D&&) = default;

		int width    = 0;
		int height   = 0;
		int channels = 0;

		// Returns the red component at position GetPosition(x, y)
		u8 GetRed(int x, int y) const;
		// Returns the green component at GetPosition(x, y) + 1
		u8 GetGreen(int x, int y) const;
		// Returns the blue component at GetPosition(x, y) + 2
		u8 GetBlue(int x, int y) const;
		// Returns ARGB value with alpha = 0
		u32 GetARGB(int x, int y) const;
	private:
		u8* m_data;

		// Returns position = (y * width + x) * channels
		ssize GetPosition(int x, int y) const;
		// Bounds Check
		bool CheckBounds(int x, int y) const;
	};
}


#endif