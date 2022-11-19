#include "effect.h"

Effect::Effect(uint8_t z_index) : z_index(z_index)
{

}

uint8_t Effect::get_z_index(void)
{
	return z_index;
}

void Effect::set_tick(unsigned long long set_tick)
{
	tick = set_tick;
}

unsigned long long Effect::get_tick(void)
{
	return tick;
}