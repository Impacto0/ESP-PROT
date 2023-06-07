#include <Arduino.h>

// EPM - ESP Protocol Message
typedef struct EPM {
    unsigned int ClientID : 4; // 4 bity na ID klienta (0-15)
    unsigned int Type : 1; // 1 bit na typ (0 - zapytanie, 1 - odpowiedź)
    unsigned int Data : 24; // 24 bity na dane (0-16777215)
} EPM;

class ESPProt
{
public:
    // metoda Init() jest wywoływana w setup() i inicjalizuje komunikację (używana po stronie klienta i serwera)
    // isServer - czy urządzenie jest serwerem (true) czy klientem (false)
    // metoda inicjalizuje WiFi i ESP-NOW
    void Init(bool isServer);

    // metoda RequestSensorData() używana jest po stronie serwera
    // wysyła zapytanie o dane do klienta
    void RequestSensorData(int type, int sensorID);

    // metoda SendSensorData() używana jest po stronie klienta
    // wysyła dane do serwera
    void SendSensorData(int type, int sensorID, int data);

    // metoda AddClient() używana jest po stronie serwera
    // dodaje odbiorcę, do którego będą wysyłane dane
    void AddClient(uint8_t* mac);

    // metoda SetServer() używana jest po stronie klienta
    // ustawia nadawce danych
    void SetServer(uint8_t* mac);

    void OnDataRecv(EPM message);

    bool IsServer = false;

private:
    uint8_t clients[10][6];
    uint8_t server[6];

    int clientCount = 0;
};