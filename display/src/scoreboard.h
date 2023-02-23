#ifndef SCOREBOARD_H
# define SCOREBOARD_H

# include "definitions.h"
# include "display.h"

class Scoreboard
{
	public:
		Scoreboard(void);

		static Scoreboard *instance;
		Scoreboard(const Scoreboard& obj) = delete;

		static Scoreboard *get_instance()
		{
			if(instance)
				return instance;
			instance = new Scoreboard();
			return instance;
		}

		void clear(void);
		void draw_score(t_score score);
		void draw_wins_dots(t_score wins);
		void draw_wins(t_score wins, e_side side, float tick);
		void draw_serving_indicator(t_serve serve);

		void draw_match_won(e_side side);
		void draw_game_won(e_side side);
		void draw_screensaver();

	private:
		Display *display;
		t_screensaver screensaver;

};

#endif
