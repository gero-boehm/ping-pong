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
	bool long_press_triggered;
	bool reset_triggered;
} t_button;

typedef struct {
  uint8_t id;
  uint8_t is_high;
  uint32_t counter;
} t_transmission;

class ButtonManager
{

	public:
		ButtonManager(void) {};

		static ButtonManager *instance;
		// ButtonManager(const ButtonManager& obj) = delete;

		static ButtonManager *get_instance()
		{
			if(instance)
				return instance;
			instance = new ButtonManager();
			return instance;
		}

		void init(uint8_t pin_a, uint8_t pin_b);
		void on_tap(void (*callback)(e_button));
		void on_long_press(void (*callback)(e_button));
		void on_reset(void (*callback)(void));
		void on_transmission_received(t_transmission *transmission);
		void loop(void);

	private:
		t_button button_a;
		t_button button_b;

		void (*callback_on_tap)(e_button);
		void (*callback_long_press)(e_button);
		void (*callback_reset)(void);
		void check_for_button_tap(t_button *button);
		void process_transmission(t_transmission *transmission, t_button *button);
		void check_end_transmission(t_button *button);
};

#endif
