#ifndef DISPLAY_H
# define DISPLAY_H

# include <stdint.h>
# include "ESP32-HUB75-MatrixPanel-I2S-DMA.h"

enum e_color : uint16_t {
	COLOR_CLEAR = 0,
	COLOR_WHITE = 65535,
	COLOR_RED = 63488,
	COLOR_GREEN = 2016,
	COLOR_BLUE = 31,
	COLOR_YELLOW = 65504,
	COLOR_MAGENTA = 63519,
	COLOR_CYAN = 2047
};

typedef struct {
	e_color color;
} t_rect_options;

typedef struct {
	uint16_t size;
	e_color color;
} t_text_options;

typedef struct {
	uint8_t color_index;
	uint16_t x;
	uint16_t y;
	uint16_t vx;
	uint16_t vy;
} t_screensaver;

class Display
{
	public:
		Display(void);

		void draw_number(uint16_t x, uint16_t y, uint16_t number, t_text_options *options);
		void draw_number_padded(uint16_t x, uint16_t y, uint16_t number, t_text_options *options);
		void draw_text(uint16_t x, uint16_t y, char *text, t_text_options *options);
		void fill_rect(uint16_t x, uint16_t y, uint16_t width, uint16_t height, t_rect_options *options);
		void clear(void);
	private:
		MatrixPanel_I2S_DMA *display;


};

#endif
