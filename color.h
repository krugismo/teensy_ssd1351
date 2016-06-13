#pragma once
#include <Arduino.h>

namespace ssd1351 {

typedef uint8_t IndexedColor;
typedef	uint16_t LowColor;
struct HighColor {
	uint8_t r;
	uint8_t g;
	uint8_t b;

	HighColor() {}
	HighColor(int16_t _r, int16_t _g, int16_t _b) : r(_r), g(_g), b(_b) {}
};

struct RGB {
	uint8_t r;
	uint8_t g;
	uint8_t b;

	uint8_t clamp(int16_t val) {
		if (val < 0) {
			return 0;
		}
		if (val > 255) {
			return 255;
		}
		return val;
	}

	RGB() {
		r = g = b = 0;
	}

	RGB(const LowColor encoded) {
		r = (encoded & 0xf800) >> 8;
		g = (encoded & 0x7E0) >> 3;
		b = (encoded & 0x1F) << 3;
	}

	RGB(const HighColor &c) {
		r = c.r;
		g = c.g;
		b = c.b;
	}

	RGB(int16_t _r, int16_t _g, int16_t _b) {
		r = clamp(_r);
		g = clamp(_g);
		b = clamp(_b);
	}

	operator HighColor() const {
		return {r, g, b};
	}

	operator LowColor() const {
		// Encode 3 byte colour into two byte (5r/6g/5b) color
		// Masks the last three bits of red/blue, last two bits of green
		// and shifts colours up to make RRRR RGGG GGGB BBBB
	  return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | ((b & 0xF8) >> 3);
	}

	operator IndexedColor() const {
		return (r & 0xE0) | (r >> 3) | (b >> 6);
	}
};
}
