#include "renderer.h"

Renderer::Renderer(void)
{

}

void Renderer::add_effect(Effect *effect)
{
	layers[effect->get_z_index()] = effect;
}

void Renderer::remove_effect(Effect *effect)
{
	layers[effect->get_z_index()] = nullptr;
}

void Renderer::render(void)
{
	Effect *effect;

	for(uint8_t i = 0; i < 256; i++)
	{
		effect = layers[i];
		if(effect == nullptr)
			continue;
		effect->render(effect->get_tick());
		effect->set_tick(effect->get_tick() + 1);
	}
}

