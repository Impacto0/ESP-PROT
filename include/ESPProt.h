#include <Arduino.h>

#ifdef ESP32
#include <WiFi.h>
#include <esp_now.h>
#else
#include <ESP8266WiFi.h>
#include <espnow.h>
#endif

// EPM - ESP Protocol Message
typedef struct EPM {
    unsigned int clientID : 4;  // 4 bity na ID klienta (0-15)
    unsigned int type : 1;      // 1 bit na typ (0 - zapytanie, 1 - odpowiedź)
    unsigned int data : 24;     // 24 bity na dane (0-16777215)
} EPM;

class ESPProt {
   public:
    // metoda Init() jest wywoływana w setup() i inicjalizuje komunikację (używana po stronie klienta i serwera)
    // isServer - czy urządzenie jest serwerem (true) czy klientem (false)
    // metoda inicjalizuje WiFi i ESP-NOW
    void init(bool isServer);

    void request(int clientID, int data);

    void send(int clientID, int data);

    // metoda AddClient() używana jest po stronie serwera
    // dodaje odbiorcę, do którego będą wysyłane dane
    void addClient(uint8_t* mac);

    // metoda SetServer() używana jest po stronie klienta
    // ustawia nadawce danych
    void setServer(uint8_t* mac);

    void initCallback() {
        esp_now_register_recv_cb(onDataRecvCallback);
    }

    static void onDataRecv(void (*callback)(EPM)) {
        // dataCallback = callback;
    }

    bool isServer = false;

   private:
    typedef void (*callback_t)(EPM);

    uint8_t clients[10][6];

    static callback_t dataCallback;

    int clientCount = 0;

#ifdef ESP32
    static void onDataRecvCallback(const uint8_t* mac, const uint8_t* data, int len) {
        EPM message;
        memcpy(&message, data, sizeof(message));
        // dataCallback(message);
    }
#else
    static void onDataRecvCallback(uint8_t* mac, uint8_t* data, uint8_t len) {
        EPM message;
        memcpy(&message, data, sizeof(message));
        // dataCallback(message);
    }
#endif
};