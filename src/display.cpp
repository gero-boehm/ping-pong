#include "display.h"

#define PANEL_RES_X 64
#define PANEL_RES_Y 32
#define PANEL_CHAIN 1

#define DEFAULT_BRIGHTNESS 50

Display::Display(void)
{
	// display config
	HUB75_I2S_CFG mxconfig(PANEL_RES_X, PANEL_RES_Y, PANEL_CHAIN);
	mxconfig.gpio.e = 32;
	mxconfig.clkphase = false;
	mxconfig.driver = HUB75_I2S_CFG::FM6124;

	// display setup
	display = new MatrixPanel_I2S_DMA(mxconfig);
	display->begin();
	display->setBrightness8(50);
	display->clearScreen();

	uint16_t COLOR_CLEAR = display->color565(0, 0, 0);
	uint16_t COLOR_WHITE = display->color565(255, 255, 255);
	uint16_t COLOR_RED = display->color565(255, 0, 0);
	uint16_t COLOR_GREEN = display->color565(0, 255, 0);
	uint16_t COLOR_BLUE = display->color565(0, 0, 255);
	uint16_t COLOR_YELLOW = display->color565(255, 255, 0);
	uint16_t COLOR_MAGENTA = display->color565(255, 0, 255);
	uint16_t COLOR_CYAN = display->color565(0, 255, 255);

	Serial.printf("COLOR_CLEAR %d\n", COLOR_CLEAR);
	Serial.printf("COLOR_WHITE %d\n", COLOR_WHITE);
	Serial.printf("COLOR_RED %d\n", COLOR_RED);
	Serial.printf("COLOR_GREEN %d\n", COLOR_GREEN);
	Serial.printf("COLOR_BLUE %d\n", COLOR_BLUE);
	Serial.printf("COLOR_YELLOW %d\n", COLOR_YELLOW);
	Serial.printf("COLOR_MAGENTA %d\n", COLOR_MAGENTA);
	Serial.printf("COLOR_CYAN %d\n", COLOR_CYAN);
}

void Display::clear(void)
{
	display->clearScreen();
}

void Display::draw_number(uint16_t x, uint16_t y, uint16_t number, t_text_options *options)
{
	char buffer[3] = {'0', '0', 0};
	int offset = number < 10;
	itoa(number, buffer + offset, 10);
	// Serial.println(buffer);
	draw_text(x, y, buffer, options);
}

void Display::draw_text(uint16_t x, uint16_t y, char *text, t_text_options *options)
{
	display->setTextSize(options->size);
	display->setTextColor(options->color);
	display->setCursor(x, y);
	display->println(text);
}

void Display::fill_rect(uint16_t x, uint16_t y, uint16_t width, uint16_t height, t_rect_options *options)
{
	display->drawRect(x, y, width, height, options->color);
}
