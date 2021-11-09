#ifndef MONOFONT_H
#define MONOFONT_H

#include "Font.h"

class MonoFont : public Font {
private:
	static constexpr uint8_t buffer_[875] = { 0b00000000, 0b00100000, 0b10100001, 0b01000001, 0b00000000, 0b10001100, 0b00001000, 0b00001000, 0b01000000, 0b00000000, 0b10000010, 0b10001111, 0b10001110, 0b00100010, 0b01001000, 0b00100000, 0b01000000, 0b10000000, 0b00000010, 0b00000000, 0b00010100, 0b01010000, 0b00010001, 0b00100000, 0b00000010, 0b00000001, 0b00000000, 0b00001000, 0b00000000, 0b01010000, 0b11100000, 0b10000011, 0b00000000, 0b00001000, 0b00000100, 0b00000000, 0b00100000, 0b00000001, 0b01000001, 0b01000100, 0b00010101, 0b00000000, 0b00100000, 0b00010000, 0b00000000, 0b00000000, 0b00001111, 0b10001110, 0b00100010, 0b01001000, 0b00000000, 0b01000000, 0b10000000, 0b00000010, 0b00000000, 0b00010100, 0b00010000, 0b10000000, 0b11010000, 0b00000000, 0b10000100, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000010, 0b10000000, 0b00000000, 0b00000000, 0b00000000, 0b00001000, 0b11100000, 0b10000011, 0b10000111, 0b00000100, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00100100, 0b01000110, 0b00010001, 0b00100010, 0b00101000, 0b00100000, 0b00000000, 0b00000000, 0b00000001, 0b00010001, 0b00001000, 0b00000100, 0b00001000, 0b00000000, 0b10000000, 0b00000000, 0b00000000, 0b00001000, 0b01010100, 0b00100000, 0b00100001, 0b11000000, 0b00001111, 0b10000000, 0b00011100, 0b00000000, 0b01000001, 0b00010000, 0b10000001, 0b00000000, 0b10000000, 0b00001000, 0b00110000, 0b00000000, 0b00000010, 0b00000100, 0b01000010, 0b00001000, 0b00100010, 0b00000000, 0b00100000, 0b11000000, 0b00000001, 0b00001000, 0b00001110, 0b00111110, 0b01111100, 0b01110000, 0b00000000, 0b00000001, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00001000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b01000000, 0b11111000, 0b11100011, 0b11100011, 0b10000111, 0b00000000, 0b00000000, 0b00000000, 0b00000001, 0b00000010, 0b00000100, 0b01000000, 0b10010001, 0b00100010, 0b00000000, 0b00000000, 0b00000000, 0b00000101, 0b00001111, 0b00010000, 0b00000100, 0b01000100, 0b10001000, 0b01000000, 0b10000000, 0b11000000, 0b00010100, 0b00000010, 0b01111000, 0b11111000, 0b11100001, 0b11000000, 0b00000000, 0b00001100, 0b00111110, 0b01111100, 0b00001001, 0b00010000, 0b10000100, 0b01000000, 0b10000000, 0b00000000, 0b01000000, 0b00000000, 0b01000000, 0b00100100, 0b01000100, 0b00010001, 0b00000010, 0b00010000, 0b00100000, 0b11000011, 0b11100001, 0b00001111, 0b00001110, 0b00010000, 0b00111000, 0b01110000, 0b00000000, 0b10000000, 0b11000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000100, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000111, 0b00001110, 0b00011100, 0b01111000, 0b01110001, 0b11000011, 0b11100111, 0b11000111, 0b00000000, 0b00100010, 0b01000100, 0b10001001, 0b00010010, 0b00100100, 0b10001000, 0b00010000, 0b00100010, 0b01100000, 0b00001000, 0b00010010, 0b00100100, 0b01001000, 0b00010001, 0b00100000, 0b01000000, 0b10000000, 0b01100000, 0b01000011, 0b01001111, 0b10011110, 0b00100000, 0b01000100, 0b11100001, 0b11000010, 0b00000000, 0b01000010, 0b00010101, 0b00100010, 0b01000100, 0b10000001, 0b00010010, 0b00000100, 0b00001001, 0b10000110, 0b00000000, 0b00110100, 0b10001001, 0b00010010, 0b00100100, 0b10001000, 0b00010000, 0b00100010, 0b01100000, 0b00100000, 0b01100010, 0b00100111, 0b10000111, 0b00011100, 0b00111110, 0b01000000, 0b01110000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b01000100, 0b11111001, 0b11110010, 0b00100100, 0b00001000, 0b10010001, 0b00011100, 0b01111000, 0b01110001, 0b00010000, 0b10000000, 0b10001001, 0b00010000, 0b00110110, 0b01100100, 0b10001001, 0b00010010, 0b00100100, 0b01000010, 0b00000010, 0b00101000, 0b01000000, 0b10101001, 0b01010010, 0b00100100, 0b01001000, 0b10011111, 0b00001000, 0b00001000, 0b11000001, 0b00000010, 0b00100100, 0b11001000, 0b10011110, 0b00100010, 0b01000100, 0b00100000, 0b00100010, 0b10000100, 0b00001000, 0b10010001, 0b00100010, 0b01000000, 0b10001001, 0b00010000, 0b10000100, 0b10001001, 0b00010000, 0b00100010, 0b01000100, 0b10001001, 0b00000010, 0b01000100, 0b01001111, 0b10001100, 0b00100010, 0b01111100, 0b10001001, 0b00010001, 0b11000100, 0b00000110, 0b10000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000111, 0b10000111, 0b10011111, 0b00100010, 0b01000100, 0b10001001, 0b00010010, 0b00100111, 0b11000111, 0b00010001, 0b00100000, 0b00010000, 0b10001001, 0b00010010, 0b00100100, 0b01001000, 0b10000001, 0b00010000, 0b01000100, 0b10000000, 0b01000010, 0b00100100, 0b01001000, 0b10001010, 0b00010100, 0b00001000, 0b01000001, 0b11100001, 0b11000001, 0b00001000, 0b10001010, 0b00100010, 0b00010000, 0b00100000, 0b01000001, 0b00000101, 0b00000000, 0b10000100, 0b00100010, 0b00101000, 0b10101000, 0b10100000, 0b10000010, 0b00000100, 0b00010010, 0b00000010, 0b00010000, 0b10001000, 0b01000010, 0b10100100, 0b01000010, 0b00010000, 0b00010000, 0b01000100, 0b11110000, 0b01000001, 0b11000001, 0b00000101, 0b00010001, 0b00001000, 0b01111100, 0b01110000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b01000000, 0b01110000, 0b01000000, 0b00000010, 0b00000000, 0b00010000, 0b00000000, 0b00000100, 0b00000001, 0b00000000, 0b01000010, 0b10000000, 0b00000100, 0b00000000, 0b01000000, 0b00000000, 0b00010000, 0b00000010, 0b00000001, 0b00000000, 0b00000000, 0b00000000, 0b01110001, 0b11100001, 0b11100011, 0b11000111, 0b00000100, 0b00000100, 0b00000000, 0b00000000, 0b00000000, 0b00100100, 0b01001000, 0b00010001, 0b00100010, 0b00001000, 0b00010000, 0b00000000, 0b00000000, 0b00000111, 0b10010001, 0b00100000, 0b01000100, 0b11111000, 0b00010000, 0b01000000, 0b00000000, 0b00000000, 0b00100010, 0b01000100, 0b10000001, 0b00010010, 0b00000000, 0b01000111, 0b00000000, 0b00111110, 0b00000000, 0b01111001, 0b11100001, 0b11100011, 0b11000111, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000001, 0b11000000, 0b00010000, 0b00001000, 0b00000100, 0b10000001, 0b10000000, 0b00000000, 0b00000000, 0b00001000, 0b00000000, 0b01000000, 0b00000000, 0b00000010, 0b00000010, 0b00000000, 0b00000000, 0b00000000, 0b00100000, 0b01110001, 0b00000001, 0b10000000, 0b11001000, 0b10001000, 0b00010100, 0b01111000, 0b01110000, 0b10000010, 0b00100111, 0b10000010, 0b00000001, 0b00100100, 0b00100000, 0b10101001, 0b00010010, 0b00100111, 0b10001000, 0b10010001, 0b00001000, 0b00000100, 0b11100000, 0b10000010, 0b10100100, 0b01001000, 0b10001000, 0b00100010, 0b01000100, 0b00100000, 0b00010010, 0b01000010, 0b00001000, 0b10010001, 0b00100010, 0b00100000, 0b01111001, 0b00010011, 0b11100000, 0b01001000, 0b10000111, 0b00100010, 0b01000100, 0b01110000, 0b00000000, 0b00100000, 0b00000000, 0b00010001, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000111, 0b00000000, 0b00000000, 0b00111000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000111, 0b10000111, 0b10010110, 0b00011100, 0b00100000, 0b10001001, 0b00010010, 0b00100100, 0b01001000, 0b10010001, 0b00100010, 0b01100100, 0b10000001, 0b11100010, 0b00100100, 0b01001000, 0b10001010, 0b00100010, 0b01000100, 0b10001001, 0b00000001, 0b11000010, 0b00001000, 0b10001010, 0b00101010, 0b00010000, 0b10001001, 0b00010010, 0b00100100, 0b00000000, 0b10001000, 0b00100010, 0b00101000, 0b10101000, 0b10100010, 0b00100111, 0b10000111, 0b10010000, 0b00011100, 0b00011100, 0b01110000, 0b01000001, 0b01000100, 0b01000111, 0b10010000, 0b00000010, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000010, 0b01000000, 0b00001000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b01110000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000011, 0b00000100, 0b00011000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00010000, 0b00010000, 0b00010000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b01111100, 0b01000000, 0b01000000, 0b01000010, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00100010, 0b00000001, 0b00000000, 0b10010101, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000001, 0b00000100, 0b00000100, 0b00000100, 0b00001000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00001000, 0b00010000, 0b00010000, 0b00010000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b01111100, 0b00110000, 0b01000001, 0b10000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000 };

public:
	MonoFont() : Font(
	70, // width
	100, // height
	7, // charWidth
	10, // charHeight
	buffer_ // buffer
	) {}
};

constexpr uint8_t MonoFont::buffer_[]; // Provide definition

#endif // MONOFONT_H