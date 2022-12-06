#include "scoreboard.h"

// MatrixPanel_I2S_DMA *dma_display = nullptr;

// uint16_t COLOR_CLEAR = display->color565(0, 0, 0);
// uint16_t COLOR_WHITE = display->color565(255, 255, 255);
// uint16_t COLOR_RED = display->color565(255, 0, 0);
// uint16_t COLOR_GREEN = display->color565(0, 255, 0);
// uint16_t COLOR_BLUE = display->color565(0, 0, 255);
// uint16_t COLOR_YELLOW = display->color565(255, 255, 0);
// uint16_t COLOR_MAGENTA = display->color565(255, 0, 255);
// uint16_t COLOR_CYAN = display->color565(0, 255, 255);

Scoreboard::Scoreboard(void)
{
	display = new Display();
	full_reset();
	render();
}

void Scoreboard::full_reset(void)
{
	starting_side = SIDE_NONE;
	serving_side = starting_side;
	score.side_a = 0;
	score.side_b = 0;
	wins.side_a = 0;
	wins.side_b = 0;
}

void Scoreboard::reset_points(void)
{
	serving_side = starting_side;
	score.side_a = 0;
	score.side_b = 0;
}

void Scoreboard::set_starting_side(e_side side)
{
	starting_side = side;
}

e_side Scoreboard::get_starting_side(void)
{
	return starting_side;
}

void Scoreboard::increment_score_for(e_side side)
{
	if(side == SIDE_A)
		score.side_a++;
	else
		score.side_b++;
}

void Scoreboard::decrement_score_for(e_side side)
{
	if(side == SIDE_A)
	{
		if(score.side_a > 0)
			score.side_a--;
	}
	else
	{
		if(score.side_b > 0)
			score.side_b--;
	}
}

void Scoreboard::increment_wins_for(e_side side)
{
	if(side == SIDE_A)
		wins.side_a++;
	else
		wins.side_b++;
}

void Scoreboard::decrement_wins_for(e_side side)
{
	if(side == SIDE_A)
	{
		if(wins.side_a > 0)
			wins.side_a--;
	}
	else
	{
		if(wins.side_b > 0)
			wins.side_b--;
	}
}

void Scoreboard::swap_wins(void)
{
	uint16_t tmp = wins.side_a;

	wins.side_a = wins.side_b;
	wins.side_b = tmp;
}

void Scoreboard::render(void)
{
	uint16_t sx = 5;
	uint16_t sy = 5;

	t_text_options text_options = {2, COLOR_WHITE};

	display->clear();
	display->draw_number(sx, sy, score.side_a, &text_options);
	display->draw_text(sx + 22, sy, ":", &text_options);
	display->draw_number(sx + 32, sy, score.side_b, &text_options);

	if(get_starting_side() != SIDE_NONE)
	{
		draw_serving_side();
		draw_wins();
	}
}

void Scoreboard::update_serving_side()
{
	bool toggle;
	uint16_t sum = score.side_a + score.side_b;
	if (sum < 20)
		serving_side = (sum / 2) % 2 == 0 ? starting_side : starting_side == SIDE_A ? SIDE_B : SIDE_A;
	else
		serving_side = sum % 2 == 0 ? starting_side : starting_side == SIDE_A ? SIDE_B : SIDE_A;
}

void Scoreboard::draw_serving_side(void)
{
	t_rect_options rect_options = {COLOR_GREEN};

	if(serving_side == SIDE_A)
		display->fill_rect(5, 21, 22, 2, &rect_options);
	else
		display->fill_rect(5 + 32, 21, 22, 2, &rect_options);
}

void Scoreboard::draw_wins(void)
{
	t_rect_options rect_options = {COLOR_RED};

	for (uint8_t i = 0; i < wins.side_a; i++)
	{
		display->fill_rect(5 + i * 4, 25, 2, 2, &rect_options);
	}
	for (uint8_t i = 0; i < wins.side_b; i++)
	{
		display->fill_rect(5 + i * 4 + 32, 25, 2, 2, &rect_options);
	}
}

e_side Scoreboard::get_match_winner(void)
{
	int difference = static_cast<int>(score.side_a) - static_cast<int>(score.side_b);

	if(abs(difference) < 2)
		return SIDE_NONE;
	if(difference > 0 && score.side_a >= 11)
		return SIDE_A;
	if(difference < 0 && score.side_b >= 11)
		return SIDE_B;
	return SIDE_NONE;
}

e_side Scoreboard::get_game_winner(void)
{
	if(wins.side_a == 2)
		return SIDE_A;
	if(wins.side_b == 2)
		return SIDE_B;
	return SIDE_NONE;
}
