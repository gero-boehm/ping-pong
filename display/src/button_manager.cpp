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

void process_tap(Game *game, e_side side)
{
	if(game->get_phase() == SCREENSAVER)
	{
		game->reset();
		game->render();
		return;
	}

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

	if(game_winner != SIDE_NONE)
		game->set_phase(GAME_ENDED);
	else if(match_winner != SIDE_NONE)
		game->set_phase(MATCH_ENDED);

	game->render();
}

void process_long(Game *game, e_side side)
{
	game->decrement_score_for(side);
	game->update_serving_side();
	game->render();
}

void process_reset(Game *game)
{
	game->reset();
	game->render();
}

void ButtonManager::init()
{
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
	Game *game = Game::get_instance();
	e_phase phase = game->get_phase();

	if(phase == MATCH_ENDED || phase == GAME_ENDED)
		return;

	if(phase == INGAME)
		game->reset_ticks();

	switch (transmission->action)
	{
		case ACTION_TAP:
			process_tap(game, transmission->side);
			break;
		case ACTION_LONG:
			process_long(game, transmission->side);
			break;
		case ACTION_RESET:
			process_reset(game);
			break;
		default:
			break;
	}
}
