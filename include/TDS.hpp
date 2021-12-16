#include <Arduino.h>
#include "BaseSensor.hpp"

class TDS : public BaseSensor
{

public:
    TDS()
    {
        this->Name =  (char*) "TDS";
        this->IsAnalog = true;
        CommandCharacteristic = new BLECharacteristic("F5A6BDC2-721D-4CDA-84D1-7C0BABE477CA", BLERead | BLEWrite, ANALOG_COMMAND_SIZE, true);
        NotifyCharacteristic = new BLECharacteristic("09E87A27-D787-455C-9F36-488A6674B73F", BLERead | BLENotify, 6, true);
    }

private:
    void pre_loop()
    {
        pinMode(this->AnalogPort, INPUT);
    }
    void post_loop() {}
    void loop()
    {
        uint16_t value = analogRead(this->AnalogPort);
        uint8_t buffer[6] = { 0, };
        buffer[1] = this->AnalogPort == A0 ? 0x00 : 0x01;
        memcpy(&buffer[2], &value, sizeof(value));
        this->NotifyCharacteristic->writeValue(buffer, sizeof(buffer));
    }
};