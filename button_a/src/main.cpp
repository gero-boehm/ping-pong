#include <Arduino.h>
#include <WiFi.h>
#include <esp_now.h>

#define BUTTON 23

typedef struct {
	uint8_t id;
	uint8_t is_high;
	uint32_t counter;
} t_transmission;

uint8_t master_address[] = {0xC8, 0xC9, 0xA3, 0xD1, 0x3A, 0x58};
esp_now_peer_info_t peer;

t_transmission transmission = {0, 0, 0};
uint8_t can_send = 1;

void config_peer(esp_now_peer_info_t *peer)
{
	peer->channel = 0;  
	peer->encrypt = false;
	memcpy(peer->peer_addr, master_address, 6);
}

// callback when data is sent
void on_data_sent(const uint8_t *mac_addr, esp_now_send_status_t status) {
	transmission.is_high = 0;
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

	// config_peer(&peer);
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
		transmission.is_high = 1;

	if (transmission.is_high)
	{
		if (can_send)
		{
			transmission.counter++;
			Serial.printf("%d %d\n", transmission.is_high, transmission.counter);
			esp_now_send(0, (uint8_t *) &transmission, sizeof(t_transmission));
			can_send = 0;
		}
	}
	else if(transmission.counter)
	{
		transmission.counter = 0;
	}
}