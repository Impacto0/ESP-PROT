#include <ESPProt.h>
#include <esp_now.h>

#ifdef ESP32
  #include <WiFi.h>
#else
  #include <ESP8266WiFi.h>
#endif

esp_now_peer_info_t _peerInfo;

void OnDataRecvCallback(const uint8_t *mac, const uint8_t *data, int len);

void ESPProt::Init(bool isServer)
{
	IsServer = isServer;

	Serial.begin(115200);

	WiFi.mode(WIFI_STA);

	Serial.printf("MAC: %s\n", WiFi.macAddress().c_str());

	if(esp_now_init() != ESP_OK){
		Serial.println("Error initializing ESP-NOW");
		return;
	}

	_peerInfo.channel = 0;
	_peerInfo.encrypt = false;

	if(IsServer){
		Serial.println("Setting up server...");

		// Dodawanie klientów
		for(int i = 0; i < clientCount && i < 10; i++){
			memcpy(_peerInfo.peer_addr, clients[i], 6);
			if (esp_now_add_peer(&_peerInfo) != ESP_OK){
				Serial.printf("Failed to add client with MAC: %02X:%02X:%02X:%02X:%02X:%02X\n",
				_peerInfo.peer_addr[0], _peerInfo.peer_addr[1], _peerInfo.peer_addr[2],
				_peerInfo.peer_addr[3], _peerInfo.peer_addr[4], _peerInfo.peer_addr[5]);
				continue;
			}
		}

		// Rejestrowanie callbacka
		esp_now_register_recv_cb(OnDataRecvCallback);

		Serial.println("Server set up!");
	}
	else{
		Serial.println("Setting up client...");

		// Dodawanie serwera
		memcpy(_peerInfo.peer_addr, server, 6);
		if (esp_now_add_peer(&_peerInfo) != ESP_OK){
			Serial.printf("Failed to add server with MAC: %02X:%02X:%02X:%02X:%02X:%02X\n",
			_peerInfo.peer_addr[0], _peerInfo.peer_addr[1], _peerInfo.peer_addr[2],
			_peerInfo.peer_addr[3], _peerInfo.peer_addr[4], _peerInfo.peer_addr[5]);
		}

		// Rejestrowanie callbacka
		esp_now_register_recv_cb(OnDataRecvCallback);

		Serial.println("Client set up!");
	}
}

void ESPProt::RequestSensorData(int type, int sensorID)
{
	// ...
}

void ESPProt::SendSensorData(int type, int sensorID, int data)
{
	// ...
}

void OnDataRecvCallback(const uint8_t *mac, const uint8_t *data, int len)
{
	// ...
}
//TEST