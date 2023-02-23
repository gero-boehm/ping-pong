#ifndef DEFINITIONS_H
# define DEFINITIONS_H

# include <stdint.h>

enum e_side {SIDE_NONE, SIDE_A, SIDE_B};
enum e_phase {INGAME, MATCH_ENDED, GAME_ENDED, SCREENSAVER};

typedef struct
{
	uint8_t side_a;
	uint8_t side_b;

}	t_score;

typedef struct {
	e_side starting_side;
	e_side serving_side;
	uint8_t serves;
	uint8_t total_serves;
}	t_serve;

#endif