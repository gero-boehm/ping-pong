#include <Arduino.h>
#include <WiFi.h>
#include <esp_now.h>

#define input 23

int status;
int last = HIGH;

// REPLACE WITH YOUR ESP RECEIVER'S MAC ADDRESS
uint8_t broadcastAddress[] = {0xC8, 0xC9, 0xA3, 0xD1, 0x3A, 0x58};

typedef struct {
	uint8_t type;
	unsigned long long pressed_for;
} t_transmission;

t_transmission transmission = {0, 0};

esp_now_peer_info_t peerInfo;

// callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
	char macStr[18];
	Serial.print("Packet to: ");
	// Copies the sender mac address to a string
	snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
					 mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
	Serial.print(macStr);
	Serial.print(" send status:\t");
	Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}
 
void setup() {
	Serial.begin(9600);

	setCpuFrequencyMhz(80);

	pinMode(input, INPUT_PULLUP);

	WiFi.mode(WIFI_STA);
 
	if (esp_now_init() != ESP_OK) {
		Serial.println("Error initializing ESP-NOW");
		return;
	}
	
	esp_now_register_send_cb(OnDataSent);
	 
	// register peer
	peerInfo.channel = 0;  
	peerInfo.encrypt = false;
	// register first peer  
	memcpy(peerInfo.peer_addr, broadcastAddress, 6);
	if (esp_now_add_peer(&peerInfo) != ESP_OK){
		Serial.println("Failed to add peer");
		return;
	}
}

void loop() {
	status = digitalRead(input);
	if(status == LOW){
			esp_now_send(0, (uint8_t *) &transmission, sizeof(t_transmission));
			transmission.pressed_for += 1;
			delay(10);
	}
	if(status == HIGH)
		transmission.pressed_for = 0;
	// if(status == HIGH && last == LOW)
	//   esp_now_send(0, (uint8_t *) &test, sizeof(test_struct));
}