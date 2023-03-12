#ifndef BUTTON_MANAGER_H
# define BUTTON_MANAGER_H

# include <stdint.h>
#include "definitions.h"

enum e_action {ACTION_TAP, ACTION_LONG, ACTION_RESET};

typedef struct {
	e_side side;
	e_action action;
} t_transmission;

class ButtonManager
{

	public:
		ButtonManager(void) {};

		static ButtonManager *instance;
		ButtonManager(const ButtonManager& obj) = delete;

		static ButtonManager *get_instance()
		{
			if(instance)
				return instance;
			instance = new ButtonManager();
			return instance;
		}

		void init();
		void on_transmission_received(t_transmission *transmission);
};

#endif
