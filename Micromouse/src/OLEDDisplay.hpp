#pragma once

#include "SSD1306Ascii.h"
#include "SSD1306AsciiAvrI2c.h"

namespace mtrn3100 {

class OLEDDisplay : public SSD1306AsciiAvrI2c {
public:
	static constexpr uint8_t SCREEN_WIDTH {128}; // OLED display width, in pixels
	static constexpr uint8_t SCREEN_HEIGHT {32}; // OLED display height, in pixels
	static constexpr uint8_t OLED_RESET {-1}; // Reset pin # (or -1 if sharing Arduino reset pin)

	OLEDDisplay(uint8_t address)
		: address(address) {}

	void setup() {
		SSD1306AsciiAvrI2c::begin(&Adafruit128x32, address);
		setFont(Adafruit5x7);
		clear();
	}

private:
	uint8_t address;
};

} // namespace mtrn3100
