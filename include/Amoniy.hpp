#include <Arduino.h>
#include "BaseSensor.hpp"

class Amoniy : public BaseSensor
{

public:
    Amoniy()
    {
        this->Name = (char *)std::string("Amoniy").c_str();
        this->IsAnalog = true;
        CommandCharacteristic = new BLECharacteristic("d10e9088-03c0-4a83-9815-f0fdeb7f30f4", BLERead | BLEWrite, ANALOG_COMMAND_SIZE, true);
        NotifyCharacteristic = new BLECharacteristic("007514df-0d8b-46e1-aaa7-3a087131d1dd", BLERead | BLENotify, 6, true);
    }

private:
    void pre_loop()
    {
        pinMode(this->AnalogPort, INPUT);
    }
    void post_loop() {}
    void loop()
    {
        uint32_t value = analogRead(this->AnalogPort);
        uint8_t buffer[6] = { 0, };
        buffer[1] = this->AnalogPort == A0 ? 0x00 : 0x01;
        memcpy(&buffer[2], &value, sizeof(value));
        this->NotifyCharacteristic->writeValue(buffer, sizeof(buffer));
    }
};