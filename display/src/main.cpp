#include <Arduino.h>

// Example sketch which shows how to display some patterns
// on a 64x32 LED matrix
//

#include "scoreboard.h"
#include "button_manager.h"
#include "display.h"

#define BUTTON_A_PIN 19
#define BUTTON_B_PIN 18

// #include "ESP32-HUB75-MatrixPanel-I2S-DMA.h"


// #define PANEL_RES_X 64      // Number of pixels wide of each INDIVIDUAL panel module. 
// #define PANEL_RES_Y 32     // Number of pixels tall of each INDIVIDUAL panel module.
// #define PANEL_CHAIN 1      // Total number of panels chained one to another
 
//MatrixPanel_I2S_DMA dma_display;
// MatrixPanel_I2S_DMA *dma_display = nullptr;

// uint16_t myBLACK = dma_display->color565(0, 0, 0);
// uint16_t myWHITE = dma_display->color565(255, 255, 255);
// uint16_t myRED = dma_display->color565(255, 0, 0);
// uint16_t myGREEN = dma_display->color565(0, 255, 0);
// uint16_t myBLUE = dma_display->color565(0, 0, 255);

Scoreboard *scoreboard = nullptr;
Display *display = nullptr;
ButtonManager *buttons = nullptr;

typedef struct {
	bool current;
	bool last;
} t_status;

t_status button_a = {LOW, HIGH};
t_status button_b = {LOW, HIGH};

void on_tap(e_button button)
{
	e_side side = button == BUTTON_A ? SIDE_A : SIDE_B;
	Serial.println(button);
	Serial.println(side);
	Serial.println("=?====");
	if(scoreboard->get_starting_side() == SIDE_NONE)
		scoreboard->set_starting_side(side);
	else
		scoreboard->increment_score_for(side);
	scoreboard->update_serving_side();
	scoreboard->render();
}

void on_long_press(e_button button)
{
	e_side side = button == BUTTON_A ? SIDE_A : SIDE_B;
	scoreboard->decrement_score_for(side);
	scoreboard->update_serving_side();
	scoreboard->render();
}

void setup() {
	Serial.begin(9600);

	buttons = ButtonManager::get_instance();
	buttons->init(BUTTON_A_PIN, BUTTON_B_PIN);
	buttons->on_tap(on_tap);
	buttons->on_long_press(on_long_press);
	
	scoreboard = new Scoreboard();

	


	// // Module configuration
	// HUB75_I2S_CFG mxconfig(
	// 	PANEL_RES_X,   // module width
	// 	PANEL_RES_Y,   // module height
	// 	PANEL_CHAIN    // Chain length
	// );

	// mxconfig.gpio.e = 32;
	// mxconfig.clkphase = false;
	// mxconfig.driver = HUB75_I2S_CFG::FM6124;

	// // Display Setup
	// dma_display = new MatrixPanel_I2S_DMA(mxconfig);
	// dma_display->begin();
	// dma_display->setBrightness8(100); //0-255
	// dma_display->clearScreen();
	// dma_display->fillScreen(dma_display->color565(0, 255, 255));

	// dma_display->setTextSize(2);     // size 1 == 8 pixels high
	// dma_display->setTextColor(dma_display->color444(15,15,15));

	// uint16_t sx = 5;
	// uint16_t sy = 5;

	// dma_display->setCursor(sx, sy);
	// dma_display->println("00");
	// dma_display->setCursor(sx + 22, sy);
	// dma_display->println(":");
	// dma_display->setCursor(sx + 32, sy);
	// dma_display->println("07");

	// dma_display->fillScreen(myRED);
	
	// // fix the screen with green
	// dma_display->fillRect(0, 0, dma_display->width(), dma_display->height(), dma_display->color444(0, 15, 0));
	// delay(500);

	// // draw a box in yellow
	// dma_display->drawRect(0, 0, dma_display->width(), dma_display->height(), dma_display->color444(15, 15, 0));
	// delay(500);

	// // draw an 'X' in red
	// dma_display->drawLine(0, 0, dma_display->width()-1, dma_display->height()-1, dma_display->color444(15, 0, 0));
	// dma_display->drawLine(dma_display->width()-1, 0, 0, dma_display->height()-1, dma_display->color444(15, 0, 0));
	// delay(500);

	// // draw a blue circle
	// dma_display->drawCircle(10, 10, 10, dma_display->color444(0, 0, 15));
	// delay(500);

	// // fill a violet circle
	// dma_display->fillCircle(40, 21, 10, dma_display->color444(15, 0, 15));
	// delay(500);

	// // fill the screen with 'black'
	// dma_display->fillScreen(dma_display->color444(0, 0, 0));

	//drawText(0);

}

// uint8_t cycle = 0;

// uint16_t colors[] = {
// 	myRED,
// 	myGREEN,
// 	myBLUE,
// 	myWHITE
// };

void loop() {

	buttons->loop();

	// button_a.current = digitalRead(BUTTON_A_PIN);
	// if(button_a.current == HIGH && button_a.last == LOW)
	// {
	// 	scoreboard->increment_score_for(SIDE_A);
	// 	scoreboard->swap_serving_side();
	// 	scoreboard->render();
	// }
	// button_a.last = button_a.current;

	// button_b.current = digitalRead(BUTTON_B_PIN);
	// if(button_b.current == HIGH && button_b.last == LOW)
	// {
	// 	scoreboard->increment_score_for(SIDE_B);
	// 	scoreboard->swap_serving_side();
	// 	scoreboard->render();
	// }
	// button_b.last = button_b.current;

	e_side winner = scoreboard->get_match_winner();
	if(winner != SIDE_NONE)
	{
		scoreboard->increment_wins_for(winner);
		if(scoreboard->get_game_winner() == SIDE_NONE)
		{
			delay(3500);
			scoreboard->reset_points();
		}
		scoreboard->render();
	}

	winner = scoreboard->get_game_winner();
	if(winner != SIDE_NONE)
	{
		delay(10000);
		scoreboard->full_reset();
		scoreboard->render();
	}



	// scoreboard->full_reset();

	// for (int i = 0; i < 11; i++)
	// {
	// 	scoreboard->increment_score_for(SIDE_A);
	// 	scoreboard->swap_serving_side();
	// 	// scoreboard->increment_score_for(SIDE_B);
	// 	scoreboard->render();
	// 	delay(1000);
	// }


	// dma_display->clearScreen();
	// for(int x = 0; x < 64; x++)
	// {
	// 	for(int y = 0; y < 32; y++)
	// 	{
	// 		int sum = x + y + toggle;
	// 		dma_display->drawPixel(x, y, sum % 2 ? myRED : myBLACK);
	// 	}
	// }
	// delay(1000);
	// toggle = !toggle;

	// for(int i = 0; i < PANEL_RES_X; i++)
	// {
	// 	uint16_t color = colors[cycle++ % 5];
	// 	dma_display->clearScreen();
	// 	// dma_display->drawFastVLine(i, 0, PANEL_RES_Y, myBLUE);
	// 	dma_display->drawCircle(i, PANEL_RES_Y / 2, 10, color);
	// 	delay(100);
	// }

	

//     // animate by going through the colour wheel for the first two lines
//     drawText(wheelval);
//     wheelval +=1;

//     delay(20); 
// /*
//   drawText(0);
//   delay(2000);
//   dma_display->clearScreen();
//   dma_display->fillScreen(myBLACK);
//   delay(2000);
//   dma_display->fillScreen(myBLUE);
//   delay(2000);
//   dma_display->fillScreen(myRED);
//   delay(2000);
//   dma_display->fillScreen(myGREEN);
//   delay(2000);
//   dma_display->fillScreen(myWHITE);
//   dma_display->clearScreen();
//   */
	
}