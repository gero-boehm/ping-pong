#include <Arduino.h>
#include <WiFi.h>
#include <esp_now.h>
#include "driver/rtc_io.h"

#define BUTTON GPIO_NUM_12

enum e_side {SIDE_NONE, SIDE_A, SIDE_B};
enum e_action {ACTION_TAP, ACTION_LONG, ACTION_RESET};

typedef struct {
	e_side side;
	e_action action;
} t_transmission;

e_side side = SIDE_A;

uint8_t master_address[] = {0xC8, 0xC9, 0xA3, 0xD1, 0x3A, 0x58};
esp_now_peer_info_t peer;

uint32_t tick = 0;
uint32_t debounce = 0;
uint32_t pressed_for = 0;
bool is_pressed = false;
bool can_send = true;

// callback when data is sent
void on_data_sent(const uint8_t *mac_addr, esp_now_send_status_t status) {
	can_send = true;
}

void send(e_action action)
{
	t_transmission transmission = {side, action};

	can_send = false;
	esp_now_send(0, (uint8_t *) &transmission, sizeof(t_transmission));
}

void setup() {
	setCpuFrequencyMhz(80);

	Serial.begin(9600);

	rtc_gpio_deinit(BUTTON);

	pinMode(BUTTON, INPUT_PULLUP);

	WiFi.mode(WIFI_STA);
 
	if (esp_now_init() != ESP_OK)
	{
		Serial.println("Error initializing ESP-NOW");
		while(1);
	}
	
	esp_now_register_send_cb(on_data_sent);

	peer.channel = 0;  
	peer.encrypt = false;
	memcpy(peer.peer_addr, master_address, 6);

	if (esp_now_add_peer(&peer) != ESP_OK)
	{
		Serial.println("Failed to add peer");
		while(1);
	}
}

void loop()
{
	if(is_pressed)
	{
		pressed_for++;
		debounce++;
		tick = 0;
	}

	uint8_t signal = !digitalRead(BUTTON);

	if(signal)
	{
		if(is_pressed == false)
			Serial.println("start");
		is_pressed = true;
		debounce = 0;
	}

	if(debounce >= 5000)
	{
		if(pressed_for > 1000 && pressed_for < 15000)
			send(ACTION_TAP);
		Serial.printf("pressed_for %d\n", pressed_for);
		is_pressed = false;
		pressed_for = 0;
		debounce = 0;
	}

	if(pressed_for == 50000)
		send(ACTION_LONG);
	else if(pressed_for == 100000)
		send(ACTION_RESET);


	// wait around 3 minutes to go to sleep. (100000 ticks equals roughly 3 seconds)
	if(tick == 100000 * 60)
	{
		Serial.println("deep_sleep");
		rtc_gpio_pullup_en(BUTTON);
		esp_sleep_enable_ext0_wakeup(BUTTON, LOW);
		esp_deep_sleep_start();
	}

	tick++;
}