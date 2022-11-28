#ifndef SCOREBOARD_H
# define SCOREBOARD_H

# include <stdint.h>
# include "display.h"

enum e_side {SIDE_NONE, SIDE_A, SIDE_B};
// enum e_phase {PRE_GAME, INGAME, INGAME_RUSH, MATCH_ENDED, GAME_ENDED};

typedef struct
{
	uint16_t side_a;
	uint16_t side_b;

}	t_score;

class Scoreboard
{
	public:
		Scoreboard(void);

		void full_reset(void);
		void reset_points(void);
		void set_starting_side(e_side side);
		e_side get_starting_side(void);
		void increment_score_for(e_side side);
		void decrement_score_for(e_side side);
		void increment_wins_for(e_side side);
		void decrement_wins_for(e_side side);
		void update_serving_side(void);
		e_side get_match_winner(void);
		e_side get_game_winner(void);

		void render(void);

	private:
		Display *display;
		e_side starting_side;
		e_side serving_side;
		t_score score;
		t_score wins;

		void draw_serving_side(void);
		void draw_wins(void);

};

#endif
