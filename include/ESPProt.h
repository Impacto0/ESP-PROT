#include <Arduino.h>

// EPM - ESP Protocol Message
struct EPM {
    unsigned int ID : 7; // 7 bitów na ID (0-127)
    unsigned int Type : 1; // 1 bit na typ (0 - zapytanie, 1 - odpowiedź)
    unsigned int Data : 24; // 24 bity na dane (0-16777215)
};

class ESPProt
{
public:
    // metoda Init() jest wywoływana w setup() i inicjalizuje komunikację (używana po stronie klienta i serwera)
    void Init(bool isServer);

    // metoda RequestSensorData() używana jest po stronie serwera
    void RequestSensorData(int type, int sensorID);

    // metoda SendSensorData() używana jest po stronie klienta
    void SendSensorData(int type, int sensorID, int data);
};