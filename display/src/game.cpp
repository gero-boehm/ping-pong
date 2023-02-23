#include "game.h"

Game *Game::instance = nullptr;

Game::Game(void)
{
	scoreboard = Scoreboard::get_instance();
}

void Game::reset(void)
{
	phase = INGAME;
	serve.starting_side = SIDE_NONE;
	serve.serving_side = SIDE_NONE;
	serve.serves = 2;
	serve.total_serves = 2;
	score.side_a = 0;
	score.side_b = 0;
	wins.side_a = 0;
	wins.side_b = 0;
	tick = 0;
	bounce = 0;
}

void Game::reset_points(void)
{
	serve.serving_side = serve.starting_side;
	serve.serves = 2;
	serve.total_serves = 2;
	score.side_a = 0;
	score.side_b = 0;
}

void Game::set_starting_side(e_side side)
{
	serve.starting_side = side;
	serve.serving_side = side;
}

void Game::increment_score_for(e_side side)
{
	if(side == SIDE_A)
		score.side_a++;
	else
		score.side_b++;
}

void Game::decrement_score_for(e_side side)
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

void Game::increment_wins_for(e_side side)
{
	if(side == SIDE_A)
		wins.side_a++;
	else
		wins.side_b++;
}

void Game::decrement_wins_for(e_side side)
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

void Game::swap_wins(void)
{
	uint16_t tmp = wins.side_a;

	wins.side_a = wins.side_b;
	wins.side_b = tmp;
}

void Game::update_serving_side(void)
{
	uint16_t sum = score.side_a + score.side_b;
	e_side other_side = serve.starting_side == SIDE_A ? SIDE_B : SIDE_A;
	bool determinant = sum < 20 ? (sum / 2) % 2 == 0 : sum % 2 == 0;
	bool is_single_serve = sum >= 20;

	serve.serves = is_single_serve ? 1 : 2 - (sum % 2);
	serve.total_serves = is_single_serve ? 1 : 2;
	serve.serving_side = determinant ? serve.starting_side : other_side;

	Serial.printf("serves: %u, total: %u, sum: %u\n", serve.serves, serve.total_serves, sum);
}

e_side Game::get_match_winner(void)
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

e_side Game::get_game_winner(void)
{
	if(wins.side_a == 2)
		return SIDE_A;
	if(wins.side_b == 2)
		return SIDE_B;
	return SIDE_NONE;
}

e_side Game::get_serving_side(void)
{
	return serve.serving_side;
}

void Game::set_phase(e_phase new_phase)
{
	phase = new_phase;
	tick = 0;
	bounce = 0;
}

e_phase Game::get_phase(void)
{
	return phase;
}

void Game::reset_ticks(void)
{
	tick = 0;
}

void Game::loop(void)
{
	// turn on screensaver after roughly 5 minutes
	if(tick == 1000000 * 6 * 5)
		phase = SCREENSAVER;

	// Serial.println(tick);

	if (phase == MATCH_ENDED && tick >= 500000)
	{
		reset_points();
		swap_wins();
		set_phase(INGAME);
		render();
	}
	else if (phase == GAME_ENDED && tick >= 1000000)
	{
		reset();
		render();
	}

	uint32_t speed = phase == SCREENSAVER ? 7000 : 2500;

	if(phase != INGAME && tick % speed == 0)
	{
		bounce += .1;
		render();
	}

	tick++;
}

void Game::render(void)
{
	scoreboard->clear();

	switch(phase) {
		case INGAME:
			scoreboard->draw_score(score);
			scoreboard->draw_wins_dots(wins);
			scoreboard->draw_serving_indicator(serve);
			break;
		case MATCH_ENDED:
			scoreboard->draw_wins(wins, get_match_winner(), bounce);
			scoreboard->draw_match_won(get_match_winner());
			break;
		case GAME_ENDED:
			scoreboard->draw_wins(wins, get_game_winner(), bounce);
			scoreboard->draw_game_won(get_game_winner());
			break;
		case SCREENSAVER:
			scoreboard->draw_screensaver();
			break;
	}
}
