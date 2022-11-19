#ifndef EFFECT_H
# define EFFECT_H

# include <stdint.h>

class Effect
{
	public:
		Effect(uint8_t z_index);
		~Effect(void);


		uint8_t get_z_index(void);
		void render(unsigned long long tick);
		void set_tick(unsigned long long set_tick);
		unsigned long long get_tick(void);

	private:
		uint8_t z_index;
		unsigned long long tick;
};

#endif
