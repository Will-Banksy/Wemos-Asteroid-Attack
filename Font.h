#ifndef FONT_H
#define FONT_H

#include <cstdint>

class Font {
public:
	uint8_t width = 0;
	uint8_t height = 0;
	uint8_t charWidth = 0;
	uint8_t charHeight = 0;
	const uint8_t* bitmap;

	Font(uint8_t width_, uint8_t height_, uint8_t charWidth_, uint8_t charHeight_, const uint8_t* bitmap_) :
		width(width_),
		height(height_),
		charWidth(charWidth_),
		charHeight(charHeight_),
		bitmap(bitmap_) {
	}
};

#endif // FONT_H