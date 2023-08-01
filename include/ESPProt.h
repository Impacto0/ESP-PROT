#include <Arduino.h>

// EPM - ESP Protocol Message
typedef struct EPM {
    unsigned int ClientID : 4;  // 4 bity na ID klienta (0-15)
    unsigned int Type : 1;      // 1 bit na typ (0 - zapytanie, 1 - odpowiedź)
    unsigned int Data : 24;     // 24 bity na dane (0-16777215)
} EPM;

class ESPProt {
   public:
    // metoda Init() jest wywoływana w setup() i inicjalizuje komunikację (używana po stronie klienta i serwera)
    // isServer - czy urządzenie jest serwerem (true) czy klientem (false)
    // metoda inicjalizuje WiFi i ESP-NOW
    void Init(bool isServer);

    void Request(int clientID, int data);

    void Send(int clientID, int data);

    // metoda AddClient() używana jest po stronie serwera
    // dodaje odbiorcę, do którego będą wysyłane dane
    void AddClient(uint8_t* mac);

    // metoda SetServer() używana jest po stronie klienta
    // ustawia nadawce danych
    void SetServer(uint8_t* mac);

    void InitCallback() {
        esp_now_register_recv_cb(OnDataRecvCallback);
    }

    // 1 argument is void function that has an arugment of type EPM
    static void OnDataRecv(void (*callback)(EPM)) {
        dataCallback = callback;
    }

    bool IsServer = false;

   private:
    typedef void (*callback_t)(EPM);

    uint8_t clients[10][6];
    uint8_t server[6];

    static callback_t dataCallback;

    int clientCount = 0;

    static void OnDataRecvCallback(const uint8_t* mac, const uint8_t* data, int len) {
        EPM message;
        memcpy(&message, data, sizeof(message));
        dataCallback(message);
    }
};