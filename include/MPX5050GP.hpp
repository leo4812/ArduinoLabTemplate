#include <Arduino.h>
#include "BaseSensor.hpp"

class MPX5050GP : public BaseSensor
{

public:
    MPX5050GP()
    {
        this->Name =  (char*) "MPX5050GP";
        this->IsAnalog = true;
        CommandCharacteristic = new BLECharacteristic("5375a33d-d83c-40e3-b241-8c9ca962aedb", BLERead | BLEWrite, ANALOG_COMMAND_SIZE, true);
        NotifyCharacteristic = new BLECharacteristic("02509572-8e98-4110-a253-f0691616cccf", BLERead | BLENotify, 6, true);
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