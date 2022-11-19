#ifndef BUTTON_MANAGER_H
# define BUTTON_MANAGER_H

# include <stdint.h>

enum e_button {BUTTON_A, BUTTON_B, BUTTON_BOTH};

typedef struct {
	e_button type;
	uint8_t pin;
	unsigned long long pressed_for;
	unsigned long long time_since_last_press;
	bool is_tap;
} t_button;

class ButtonManager
{
	public:
		ButtonManager(uint8_t pin_a, uint8_t pin_b);

		void on_tap(void (*callback)(e_button));
		void on_long_press(void (*callback)(e_button));
		void loop(void);

	private:
		t_button button_a;
		t_button button_b;

		void (*callback_on_tap)(e_button);
		void (*callback_long_press)(e_button);
		void check_for_button_tap(t_button *button);
};

#endif
