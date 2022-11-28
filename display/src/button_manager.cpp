#include <Arduino.h>
#include <WiFi.h>
#include <esp_now.h>
#include "button_manager.h"

t_transmission transmission;
ButtonManager* ButtonManager::instance = nullptr;

void on_data_received(const uint8_t *mac, const uint8_t *payload, int len)
{
	ButtonManager *manager = ButtonManager::get_instance();

	memcpy(&transmission, payload, len);
	manager->on_transmission_received(&transmission);
}

void ButtonManager::init(uint8_t pin_a, uint8_t pin_b)
{
	button_a = {BUTTON_A, pin_a, 0, false, false};
	button_b = {BUTTON_B, pin_b, 0, false, false};
	pinMode(pin_a, INPUT_PULLUP);
	pinMode(pin_b, INPUT_PULLUP);

	Serial.print("ESP Board MAC Address:  ");
	Serial.println(WiFi.macAddress());

	WiFi.mode(WIFI_STA);

	if(esp_now_init() != ESP_OK) {
		Serial.println("Error initializing ESP-NOW");
		while(1);
	}

	esp_now_register_recv_cb(on_data_received);
}

void ButtonManager::on_tap(void (*callback)(e_button))
{
	callback_on_tap = callback;
}

void ButtonManager::on_long_press(void (*callback)(e_button))
{
	callback_long_press = callback;
}

void ButtonManager::on_transmission_received(t_transmission *transmission)
{
	if(transmission->type == 0)
		process_transmission(transmission, &button_a);
	else
		process_transmission(transmission, &button_b);
}

void ButtonManager::process_transmission(t_transmission *transmission, t_button *button)
{
	button->pressed_for = transmission->pressed_for;
	if(button->pressed_for > 0)
		button->is_tap = true;
	if(button->pressed_for > 10)
		button->is_tap = false;
	if(!button->long_press_triggered && button->pressed_for > 20)
	{
		button->long_press_triggered = true;
		callback_long_press(button->type);
	}
	button->time_since_last_press = 0;
}

void ButtonManager::check_end_transmission(t_button *button)
{
	button->time_since_last_press++;
	if(button->time_since_last_press != 50000)
		return;
	if(button->is_tap)
		callback_on_tap(button->type);
	button->long_press_triggered = false;
}

void ButtonManager::loop(void)
{
	check_end_transmission(&button_a);
	check_end_transmission(&button_b);
}

void ButtonManager::check_for_button_tap(t_button *button)
{
	if(button->pressed_for == 0)
		callback_on_tap(button->type);
	if(button->pressed_for == 200)
		callback_long_press(button->type);
}
