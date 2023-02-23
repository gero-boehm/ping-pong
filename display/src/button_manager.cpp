#include <Arduino.h>
#include <WiFi.h>
#include <esp_now.h>
#include "button_manager.h"
#include "game.h"

ButtonManager *ButtonManager::instance = nullptr;
t_transmission transmission;

void on_data_received(const uint8_t *mac, const uint8_t *payload, int len)
{
	ButtonManager *manager = ButtonManager::get_instance();

	memcpy(&transmission, payload, len);
	manager->on_transmission_received(&transmission);
}

void ButtonManager::init(uint8_t pin_a, uint8_t pin_b)
{
	button_a = {BUTTON_A, pin_a, 0, false, false, false};
	button_b = {BUTTON_B, pin_b, 0, false, false, false};
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

void ButtonManager::on_transmission_received(t_transmission *transmission)
{
	if(transmission->id == 0)
		process_transmission(transmission, &button_a);
	else
		process_transmission(transmission, &button_b);
}

void ButtonManager::process_transmission(t_transmission *transmission, t_button *button)
{
	Game *game = Game::get_instance();

	if(game->get_phase() != INGAME && game->get_phase() != SCREENSAVER)
		return;

	if(game->get_phase() == INGAME)
		game->reset_ticks();

	e_side side = get_side_for(button);

	Serial.printf("button pressed: %d\n", transmission->counter);

	button->pressed_for = transmission->counter;
	if(button->pressed_for > 0)
		button->is_tap = true;
	if(button->pressed_for > 50)
		button->is_tap = false;
	if(!button->long_press_triggered && button->pressed_for > 75)
	{
		button->long_press_triggered = true;
		game->decrement_score_for(side);
		game->update_serving_side();
		game->render();
	}
	if(!button->reset_triggered && button->pressed_for > 150)
	{
		button->reset_triggered = true;
		game->reset();
		game->render();
	}
	button->time_since_last_press = 0;
}

void ButtonManager::check_end_transmission(t_button *button)
{
	Game *game = Game::get_instance();
	e_side side = get_side_for(button);

	button->time_since_last_press++;
	if(button->time_since_last_press < 50000)
		return;
	if(button->is_tap)
	{
		button->is_tap = false;

		if(game->get_phase() == SCREENSAVER)
		{
			game->reset();
			game->render();
			return;
		}
		// Serial.printf("is tap %d %d %d\n", side == SIDE_NONE, side == SIDE_A, side == SIDE_B);
		if(game->get_serving_side() == SIDE_NONE)
			game->set_starting_side(side);
		else
			game->increment_score_for(side);
		game->update_serving_side();

		e_side match_winner = game->get_match_winner();

		if(match_winner != SIDE_NONE)
		{
			game->increment_wins_for(match_winner);
			game->render();
		}

		e_side game_winner = game->get_game_winner();

		// Serial.printf("match_winner %d %d %d\n", match_winner == SIDE_NONE, match_winner == SIDE_A, match_winner == SIDE_B);
		// Serial.printf("game_winner %d %d %d\n", game_winner == SIDE_NONE, game_winner == SIDE_A, game_winner == SIDE_B);

		if(game_winner != SIDE_NONE)
			game->set_phase(GAME_ENDED);
		else if(match_winner != SIDE_NONE)
			game->set_phase(MATCH_ENDED);

		game->render();
	}
	button->long_press_triggered = false;
	button->reset_triggered = false;
}

void ButtonManager::loop(void)
{
	check_end_transmission(&button_a);
	check_end_transmission(&button_b);
}

e_side ButtonManager::get_side_for(t_button *button)
{
	return button->type == BUTTON_A ? SIDE_A : SIDE_B;
}
