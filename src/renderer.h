#ifndef RENDERER_H
# define RENDERER_H

# include <stdint.h>
# include "effect.h"

class Renderer
{
	public:
		Renderer(void);

		void add_effect(Effect *effect);
		void remove_effect(Effect *effect);
		void render(void);

	private:
		Effect *layers[256] {};
};

#endif
