#include <cstdint> // Integer types
#include <Wire.h> // Wire - For transmitting data
#include "MonoFont.h" // Font

#define OLED_RESET -1
#define OLED_DATA D2 // 4
#define OLED_CLOCK D1 // 5
#define OLED_I2C_ADDRESS 0x3C // Slave address of OLED for the MCU I2C Interface. The Slave Address bit (SA0) is 0, if it's 1 (0x3D) it doesn't seem to work

// OLED Dimensions & Pixel count
#define OLED_WIDTH 128
#define OLED_HEIGHT 64
#define OLED_PIXEL_COUNT 7936
#define OLED_BYTE_COUNT 1024 // Number of bytes in the buffer

// Commands - This isn't all of the commands available I don't think
#define COM_DISPLAY_OFF 0xAE
#define COM_DISPLAY_ON 0xAF
#define COM_SET_DISPLAY_CLOCK_DIV 0xD5
#define COM_SET_MULTIPLEX 0xA8
#define COM_SET_DISPLAY_OFFSET 0xD3
#define COM_SET_START_LINE 0x40
#define COM_CHARGE_PUMP 0x8D
#define COM_SET_ADDRESSING_MODE 0x20
#define COM_SET_SEGMENT_REMAP 0xA0
#define COM_SET_COM_SCAN_DIR 0xC0
#define COM_SET_PRE_CHARGE_PERIOD 0xD9
#define COM_SET_VCOMH_DESELECT_LEVEL 0xDB
#define COM_DISPLAY_ALL_ON_RESUME 0xA4
#define COM_DISPLAY_NORMAL 0xA6
#define COM_DISPLAY_INVERSE 0xA7
#define COM_ACTIVATE_SCROLL 0x2F
#define COM_DEACTIVATE_SCROLL 0x2E
#define COM_SET_COLUMN_ADDRESS 0x21
#define COM_SET_PAGE_ADDRESS 0x22
#define COM_SET_COM_PINS_CONFIGURATION 0xDA
#define COM_SET_CONTRAST 0x81

// Typedefs with semantic value instead of practical value
typedef uint8_t dims_t; // Dimensions - Size of things (size_t is taken)
typedef int16_t coord_t; // Coordinates

class SSD1306_128x64_DisplayController { // The name's a bit long, I know
public:
	uint8_t buffer[OLED_BYTE_COUNT];
	Font font = MonoFont(); // Yay can now easily do different fonts - Should I make any

	SSD1306_128x64_DisplayController(bool immediateBegin = true) {
		if(immediateBegin) {
			Begin();
		}
	}

	void SendCommand(uint8_t data) {
		// Control
		// Co - Not sure
		// D/C - Data/Command - If 0, byte after control will be interpreted as a command, otherwise as data to be stored in GDDRAM
		uint8_t control = 0x00; // Co = 0, D/C = 0
		Wire.beginTransmission(OLED_I2C_ADDRESS);
		Wire.write(control);
		Wire.write(data);
		Wire.endTransmission();
	}

	/// Basically what this function does is set all the OLED GDDRAM memory pointers to 0 and start writing into the OLED GDDRAM memory
	void Display() {
		SendCommand(COM_SET_COLUMN_ADDRESS);
		SendCommand(0); // Column start address
	    SendCommand(OLED_WIDTH - 1); // Column end address
		SendCommand(COM_SET_PAGE_ADDRESS);
		SendCommand(0); // Page start address
		SendCommand((OLED_HEIGHT / 8) - 1); // Page end address

		for(uint16_t i = 0; i < OLED_BYTE_COUNT; i++) {
			Wire.beginTransmission(OLED_I2C_ADDRESS);
			Wire.write(0x40); // D/C = 1 - Interpret as data to be stored in GDDRAM
			for(uint8_t x = 0; x < 16; x++) {
				Wire.write(buffer[i]);
				i++;
			}
			i--;
			Wire.endTransmission();
		}
	}

	void Begin() {
		Wire.begin();

		// Adafruit sends all these commands, which probably means it's a good idea for me to do as well. Don't want previous configurations of the OLED to mess with this running session

		SendCommand(COM_DISPLAY_OFF);

		SendCommand(COM_SET_DISPLAY_CLOCK_DIV);
		SendCommand(0x80); // The suggested ratio, apparently, is 0x80

		SendCommand(COM_SET_MULTIPLEX);
		SendCommand(OLED_HEIGHT - 1);

		SendCommand(COM_SET_DISPLAY_OFFSET);
		SendCommand(0x0); // no offset

		SendCommand(COM_SET_START_LINE | 0x0); // Line 0

		SendCommand(COM_CHARGE_PUMP);
		SendCommand(0x14);

		SendCommand(COM_SET_ADDRESSING_MODE);
		SendCommand(0x00); // Horizontal memory addressing mode

		SendCommand(COM_SET_SEGMENT_REMAP | 0x0); // Segment Re-mapping - To do with what memory corresponds to what position on the screen I think

		SendCommand(COM_SET_COM_SCAN_DIR | 0x8);

		SendCommand(COM_SET_COM_PINS_CONFIGURATION);
		SendCommand(0x12);

		SendCommand(COM_SET_CONTRAST);
		SendCommand(0xCF);

		SendCommand(COM_SET_PRE_CHARGE_PERIOD);
		SendCommand(0xF1); // Arbitrary? No idea

		SendCommand(COM_SET_VCOMH_DESELECT_LEVEL);
		SendCommand(0x40);

		SendCommand(COM_DISPLAY_ALL_ON_RESUME);

		SendCommand(COM_DISPLAY_NORMAL);

		SendCommand(COM_DEACTIVATE_SCROLL);

		SendCommand(COM_DISPLAY_ON);
	}

	/// Returns the bit, counting from the start of the buffer, that is at position x, y on the screen
	uint16_t BitIndex(uint8_t x, uint8_t y) {
		uint8_t pageIndex = y / 8;
		uint8_t columnIndex = OLED_WIDTH - 1 - x;
		uint8_t bit = y % 8;
		return pageIndex * OLED_WIDTH * 8 + columnIndex * 8 + bit;
	}

	/// Sets a pixel
	void SetPixel(coord_t x, coord_t y, bool value = true) {
		uint8_t i = x;
		uint8_t j = y;
		if(x != i) { return; } // Test if x and y are in the uint8_t range - If not, return
		if(y != j) { return; }
		uint16_t bIndex = BitIndex(i, j); // The location of the bit to change
		uint16_t byteIndex = (bIndex / 8); // The location of the byte containing that bit
		bIndex = (bIndex % 8); // Repurposed: The location of the bit to change inside the byte, starting from the LSB (index = 0)
		if(byteIndex < 0 || byteIndex >= OLED_BYTE_COUNT) {
			return;
		}
		if(value) {
			buffer[byteIndex] = buffer[byteIndex] | (0x1 << bIndex);
		} else {
			buffer[byteIndex] = buffer[byteIndex] & (~(0x1 << bIndex));
		}
	}

	/// Returns whether a pixel is lit or not
	bool GetPixel(coord_t x, coord_t y) {
		uint8_t i = x;
		uint8_t j = y;
		if(x != i) { return false; } // Test if x and y are in the uint8_t range - If not, return default value
		if(y != j) { return false; }
		uint16_t bIndex = BitIndex(i, j); // The location of the bit to get
		uint16_t byteIndex = (bIndex / 8); // The location of the byte containing that bit
		bIndex = (bIndex % 8); // Repurposed: The location of the bit to get inside the byte, starting from the LSB (index = 0)
		if(byteIndex < 0 || byteIndex >= OLED_BYTE_COUNT) {
			return false;
		}
		return (buffer[byteIndex] >> bIndex) & 0x1;
	}

	/// Uses Bresenham's juicy line algorithm to draw a line between the specified points
	void DrawLine(coord_t x0, coord_t y0, coord_t x1, coord_t y1, bool value = true) { // Bit of good old Bresenham's Algorithm. Copying from a previous implementation
		// Actually some optimisation would be nice - So if it's a horizontal or vertical line, Bresenham is a bit much. Maybe implement a fast horizontal/vertical function
		coord_t dx =  abs(x1 - x0);
		coord_t sx = x0 < x1 ? 1 : -1;
		coord_t dy = -abs(y1 - y0);
		coord_t sy = y0 < y1 ? 1 : -1;
		coord_t err = dx + dy;
		coord_t e2;

		while(true) {
			SetPixel(x0, y0, value);
			if (x0 == x1 && y0 == y1) break;
			e2 = 2 * err;
			if (e2 >= dy) {
				err += dy;
				x0 += sx;
			}
			if (e2 <= dx) {
				err += dx;
				y0 += sy;
			}
		}
	}

	/// The bitArray is simply an array of bytes that contain the image data in the bits - Like below shows an array with a byte on each line. The image that it represents is 8x8, each bit is a pixel
	/// 11111111
	/// 10000001
	/// 10111101
	/// 10100101
	/// 10100101
	/// 10111101
	/// 10000001
	/// 11111111
	/// width and height are the dimensions of the bitmap, in bits
	/// x and y are the position to put the bitmap on the screen
	void DrawBitmap(const uint8_t* bitArray, dims_t width, dims_t height, coord_t x, coord_t y, bool onlyDrawTrue = false, bool inverted = false) {
		DrawBitmap(bitArray, width, height, x, y, 0, 0, width, height, onlyDrawTrue, inverted);
	}

	/// Draws a subsection of a bitmap
	/// Look at other bitmap drawing function for details on how bitmaps work
	/// width and height are the dimensions of the bitmap, in bits
	/// x and y are the position to put the bitmap on the screen
	/// subsectX and subsectY are the top-left corner of the rectangle in the bitmap to draw
	/// subsectWidth and subsectHeight are the dimensions of that rectangle
	void DrawBitmap(const uint8_t* bitArray, dims_t width, dims_t height, coord_t x, coord_t y, coord_t subsectX, coord_t subsectY, dims_t subsectWidth, dims_t subsectHeight, bool onlyDrawTrue = false, bool inverted = false) {
		auto bitmapIndex = [](coord_t i, coord_t j, coord_t width) { // A very simple lambda function to have working out the index slightly less messy
			return coord_t(i + j * width);
		};

		// Here it looks like a lot but it's simply getting indexes to pixels in the image and where it's writing them to on the screen
		// Then in the for loops it's simply looping through those indexes and copying each bit from the bitmap to the screen
		coord_t imgI = subsectX;
		coord_t screenI = x;
		for(; imgI < (subsectX + subsectWidth) && screenI < (x + width) && screenI < OLED_WIDTH; imgI++, screenI++) {
			coord_t imgJ = subsectY;
			coord_t screenJ = y;
			for(; imgJ < (subsectY + subsectHeight) && screenJ < (y + height) && screenJ < OLED_HEIGHT; imgJ++, screenJ++) {
				uint8_t i = imgI;
				uint8_t j = imgJ;
				if(imgI != i) { continue; }  // Test if x and y are in the uint8_t range - If not, return
				if(imgJ != j) { continue; }
				uint16_t bmIndex = bitmapIndex(i, j, width);
				uint16_t bmByteIndex = bmIndex / 8;
				uint8_t bitNum = 7 - (bmIndex % 8);
				bool bit = (bitArray[bmByteIndex] >> bitNum) & 0x1;
				bool value = inverted ? !bit : bit;
				if(onlyDrawTrue && !bit) {
				} else {
					SetPixel(screenI, screenJ, value);
				}
			}
		}
	}

	void DrawChar(char ch, coord_t x, coord_t y, bool value = true) {
		uint8_t chVal = ch - 32;
		uint8_t i = chVal;
		uint8_t j = 0;
		while(i >= 10) {
			i -= 10;
			j++;
		}
		i *= font.charWidth;
		j *= font.charHeight;
		DrawBitmap(font.bitmap, font.width, font.height, x, y, i, j, font.charWidth, font.charHeight, true, !value);
	}

	void DrawString(String str, coord_t x_, coord_t y_, bool value = true, bool wrap = false) {
		coord_t x = x_;
		coord_t y = y_;
		for(int i = 0; i < str.length(); i++) {
			DrawChar(str[i], x, y, value);
			x += font.charWidth;
			if(wrap && (x + font.charWidth) > OLED_WIDTH) {
				y += font.charHeight;
				x = x_;
			} else if(!wrap && x > OLED_WIDTH) {
				return; // We're not going to be rendering any more visible text, so just quit
			}
		}
	}

	void DrawRect(coord_t x, coord_t y, dims_t width, dims_t height, bool value = true) {
		DrawLine(x, y, x + width - 1, y, value);
		DrawLine(x, y, x, y + height - 1, value);
		DrawLine(x + width - 1, y, x + width - 1, y + height - 1, value);
		DrawLine(x, y + height - 1, x + width - 1, y + height - 1, value);
	}

	void FillRect(coord_t x, coord_t y, dims_t width, dims_t height, bool value = true) {
		for(coord_t i = x; i < (x + width); i++) {
			for(coord_t j = y; j < (y + height); j++) {
				SetPixel(i, j, value);
			}
		}
	}

	// Implement other functions as I need them

	/// Simply sets the whole buffer to 0
	void ClearDisplay() {
		memset(buffer, 0, OLED_BYTE_COUNT); // Set the buffer to 0
	}
};