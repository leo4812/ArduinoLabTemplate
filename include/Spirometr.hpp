#include <Arduino.h>
#include "BaseSensor.hpp"

class Spirometr : public BaseSensor
{

public:
    Spirometr()
    {
        this->Name = (char *)std::string("Spirometr").c_str();
        this->IsAnalog = true;
        CommandCharacteristic = new BLECharacteristic("3b09133f-d9e0-480d-890c-e658f5baf74a", BLERead | BLEWrite, ANALOG_COMMAND_SIZE, true);
        NotifyCharacteristic = new BLECharacteristic("e023b1fa-b8ac-4a20-af69-448c78e15639", BLERead | BLENotify, 6, true);
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