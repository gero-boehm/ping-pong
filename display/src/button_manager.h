#ifndef BUTTON_MANAGER_H
# define BUTTON_MANAGER_H

# include <stdint.h>
#include "definitions.h"

enum e_button {BUTTON_A, BUTTON_B, BUTTON_BOTH};

typedef struct {
	e_button type;
	uint8_t pin;
  	unsigned long long pressed_for;
	unsigned long long time_since_last_press;
	bool is_tap;
	bool long_press_triggered;
	bool reset_triggered;
} t_button;

typedef struct {
  uint8_t id;
  uint32_t counter;
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

		void init(uint8_t pin_a, uint8_t pin_b);
		void on_transmission_received(t_transmission *transmission);
		void loop(void);

	private:
		t_button button_a;
		t_button button_b;

		void process_transmission(t_transmission *transmission, t_button *button);
		void check_end_transmission(t_button *button);
		e_side get_side_for(t_button *button);
};

#endif
