#ifndef GAME_H
# define GAME_H

# include "definitions.h"
# include "scoreboard.h"

class Game
{
	public:
		Game(void);

		static Game *instance;
		Game(const Game& obj) = delete;

		static Game *get_instance()
		{
			if(instance)
				return instance;
			instance = new Game();
			return instance;
		}

		void reset(void);
		void reset_points(void);
		void set_starting_side(e_side side);
		void increment_score_for(e_side side);
		void decrement_score_for(e_side side);
		void increment_wins_for(e_side side);
		void decrement_wins_for(e_side side);
		void swap_wins(void);
		void update_serving_side(void);
		e_side get_serving_side(void);

		e_side get_match_winner(void);
		e_side get_game_winner(void);

		void set_phase(e_phase new_phase);
		e_phase get_phase(void);

		void reset_ticks(void);

		void loop(void);
		void render(void);

	private:
		Scoreboard *scoreboard;
		e_phase phase;
		t_serve serve;
		t_score score;
		t_score wins;
		uint32_t tick;
		float bounce;

};

#endif
