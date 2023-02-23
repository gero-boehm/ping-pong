#include <Arduino.h>
#include <WiFi.h>
#include <esp_now.h>
#include "driver/rtc_io.h"

#define BUTTON GPIO_NUM_12

typedef struct {
	uint8_t id;
	uint32_t counter;
} t_transmission;

uint8_t master_address[] = {0xC8, 0xC9, 0xA3, 0xD1, 0x3A, 0x58};
esp_now_peer_info_t peer;

t_transmission transmission = {1, 0};
uint8_t is_high;
uint8_t can_send = 1;
uint32_t tick = 0;

// callback when data is sent
void on_data_sent(const uint8_t *mac_addr, esp_now_send_status_t status) {
	is_high = 0;
	can_send = 1;
}
 
void setup() {
	setCpuFrequencyMhz(80);

	Serial.begin(9600);

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
	if(!digitalRead(BUTTON))
		is_high = 1;

	if (is_high)
	{
		tick = 0;

		if (can_send)
		{
			transmission.counter++;
			esp_now_send(0, (uint8_t *) &transmission, sizeof(t_transmission));
			can_send = 0;
		}
	}
	else if(transmission.counter)
	{
		transmission.counter = 0;
	}

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