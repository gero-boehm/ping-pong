#ifndef GAME_H
# define GAME_H

enum e_side {SIDE_NONE, SIDE_A, SIDE_B};

typedef struct
{
	uint8_t side_a;
	uint8_t side_b;

}	t_score;

class Game
{
	public:
		void reset(void);

	private:

};

#endif
