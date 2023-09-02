#include <ESPProt.h>

#ifdef ESP32
#include <WiFi.h>
#include <esp_now.h>
#else
#include <ESP8266WiFi.h>
#include <espnow.h>
#endif

#ifdef ESP32
esp_now_peer_info_t _peerInfo;

void ESPProt::init(bool isServer) {
    isServer = isServer;

    Serial.begin(115200);

    WiFi.mode(WIFI_STA);

    Serial.printf("MAC: %s\n", WiFi.macAddress().c_str());

    if (esp_now_init() != ESP_OK) {
        Serial.println("Error initializing ESP-NOW");
        return;
    }

    _peerInfo.channel = 0;
    _peerInfo.encrypt = false;

    if (isServer) {
        Serial.println("Setting up server...");

        // Dodawanie klientów
        for (int i = 0; i < clientCount && i < 10; i++) {
            memcpy(_peerInfo.peer_addr, clients[i], 6);
            if (esp_now_add_peer(&_peerInfo) != ESP_OK) {
                Serial.printf("Failed to add client with MAC: %02X:%02X:%02X:%02X:%02X:%02X\n",
                              _peerInfo.peer_addr[0], _peerInfo.peer_addr[1], _peerInfo.peer_addr[2],
                              _peerInfo.peer_addr[3], _peerInfo.peer_addr[4], _peerInfo.peer_addr[5]);
                continue;
            }
        }

        // Rejestrowanie callbacka
        initCallback();

        Serial.println("Server set up!");
    } else {
        Serial.println("Setting up client...");

        // Rejestrowanie callbacka
        initCallback();

        Serial.println("Client set up!");
    }
}
#else
void ESPProt::init(bool isServer) {
    isServer = isServer;

    Serial.begin(115200);

    WiFi.mode(WIFI_STA);

    Serial.printf("MAC: %s\n", WiFi.macAddress().c_str());

    if (esp_now_init() != 0) {
        Serial.println("Error initializing ESP-NOW");
        return;
    }

    if (isServer) {
        Serial.println("Setting up server...");

        // Dodawanie klientów
        for (int i = 0; i < clientCount && i < 10; i++) {
            if (esp_now_add_peer(clients[i], ESP_NOW_ROLE_SLAVE, 1, NULL, 0) != 0) {
                Serial.printf("Failed to add client with MAC: %02X:%02X:%02X:%02X:%02X:%02X\n",
                              clients[i][0], clients[i][1], clients[i][2],
                              clients[i][3], clients[i][4], clients[i][5]);
                continue;
            }
        }

        // Rejestrowanie callbacka
        initCallback();

        Serial.println("Server set up!");
    } else {
        Serial.println("Setting up client...");

        // Dodawanie serwera
        esp_now_set_self_role(ESP_NOW_ROLE_SLAVE);

        // Rejestrowanie callbacka
        initCallback();

        Serial.println("Client set up!");
    }
}
#endif

void ESPProt::request(int clientID, int data) {
    EPM message;

    message.clientID = clientID;
    message.type = 0;
    message.data = data;

    esp_now_send(NULL, (uint8_t *)&message, sizeof(EPM));
}

void ESPProt::send(int clientID, int data) {
    EPM message;

    message.clientID = clientID;
    message.type = 1;
    message.data = data;

    esp_now_send(NULL, (uint8_t *)&message, sizeof(EPM));
}
