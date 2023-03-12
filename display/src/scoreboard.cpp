#include "game.h"
#include "scoreboard.h"
#include "math.h"

Scoreboard *Scoreboard::instance = nullptr;
uint16_t padding = 5;

Scoreboard::Scoreboard(void)
{
	display = new Display();
	screensaver = {0, 0, 0, 1, 1};
}

void Scoreboard::clear(void)
{
	display->clear();
}

void Scoreboard::draw_score(t_score score)
{
	t_text_options text_options = {2, COLOR_WHITE};

	display->draw_number_padded(padding, padding, score.side_a, &text_options);
	display->draw_text(padding + 22, padding, ":", &text_options);
	display->draw_number_padded(padding + 32, padding, score.side_b, &text_options);
}

void Scoreboard::draw_wins_dots(t_score wins)
{
	t_rect_options rect_options = {COLOR_RED};

	for (uint8_t i = 0; i < wins.side_a; i++)
	{
		display->fill_rect(padding + i * 4, 25, 2, 2, &rect_options);
	}
	for (uint8_t i = 0; i < wins.side_b; i++)
	{
		display->fill_rect(padding + i * 4 + 32, 25, 2, 2, &rect_options);
	}
}

void Scoreboard::draw_wins(t_score wins, e_side side, float tick)
{
	t_text_options text_options = {2, COLOR_WHITE};

	uint16_t bounce = abs(sin(tick)) * 4;
	uint16_t bounce_a = side == SIDE_A ? bounce : 0;
	uint16_t bounce_b = side == SIDE_B ? bounce : 0;


	display->draw_number(padding + 6, padding - bounce_a, wins.side_a, &text_options);
	display->draw_text(padding + 22, padding, ":", &text_options);
	display->draw_number(padding + 38, padding - bounce_b, wins.side_b, &text_options);
}

void Scoreboard::draw_serving_indicator(t_serve serve)
{
	if(serve.serving_side == SIDE_NONE)
		return;

	t_rect_options rect_options = {COLOR_GREEN};

	uint16_t x = padding + (serve.serving_side == SIDE_A ? 0 : 32);

	if(serve.total_serves == 2)
	{
		display->fill_rect(x, 21, 10, 2, &rect_options);
		if(serve.serves == 2)
			display->fill_rect(x + 12, 21, 10, 2, &rect_options);
	}
	else
	{
		display->fill_rect(x, 21, 22, 2, &rect_options);
	}
}

void Scoreboard::draw_match_won(e_side side)
{
	t_text_options text_options = {1, COLOR_YELLOW};

	display->draw_text(padding, padding + 16, "MATCH WON", &text_options);
}

void Scoreboard::draw_game_won(e_side side)
{
	t_text_options text_options = {1, COLOR_MAGENTA};

	display->draw_text(padding, padding + 16, "GAME OVER", &text_options);
}

void Scoreboard::draw_screensaver() {
	uint16_t nx = screensaver.x + screensaver.vx;
	uint16_t ny = screensaver.y + screensaver.vy;
	
	e_color colors[6] = {COLOR_RED, COLOR_GREEN, COLOR_BLUE, COLOR_YELLOW, COLOR_MAGENTA, COLOR_CYAN};

	if(nx < 0 || nx > 62)
	{
		screensaver.color_index++;
		screensaver.vx *= -1;
	}
	if(ny < 0 || ny > 30)
	{
		screensaver.color_index++;
		screensaver.vy *= -1;
	}

	screensaver.x += screensaver.vx;
	screensaver.y += screensaver.vy;	
	
	t_rect_options rect_options = {colors[screensaver.color_index % 6]};
	display->fill_rect(screensaver.x, screensaver.y, 2, 2, &rect_options);
}