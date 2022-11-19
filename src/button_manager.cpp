#include <Arduino.h>
#include "button_manager.h"

ButtonManager::ButtonManager(uint8_t pin_a, uint8_t pin_b)
{
	button_a = {BUTTON_A, pin_a, 0, 0, false};
	button_b = {BUTTON_B, pin_b, 0, 0, false};
	pinMode(pin_a, INPUT_PULLUP);
	pinMode(pin_b, INPUT_PULLUP);
}

void ButtonManager::on_tap(void (*callback)(e_button))
{
	callback_on_tap = callback;
}

void ButtonManager::on_long_press(void (*callback)(e_button))
{
	callback_long_press = callback;
}

void ButtonManager::loop(void)
{
	check_for_button_tap(&button_a);
	check_for_button_tap(&button_b);
}

void ButtonManager::check_for_button_tap(t_button *button)
{
	button->time_since_last_press++;
	if(!digitalRead(button->pin))
	{
		if(button->pressed_for == 0 && button->time_since_last_press > 50000)
			button->is_tap = true;
		if(button->pressed_for == 50000)
			button->is_tap = false;
		if(button->pressed_for == 100000)
			callback_long_press(button->type);
		button->pressed_for++;
		button->time_since_last_press = 0;
	}
	else
	{
		if(button->is_tap)
		{
			callback_on_tap(button->type);
			button->is_tap = false;
		}
		button->pressed_for = 0;
	}
}
