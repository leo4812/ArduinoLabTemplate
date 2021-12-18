#include <Arduino.h>
#include "BaseSensor.hpp"

class Dinamometr : public BaseSensor
{

public:
    Dinamometr()
    {
        this->Name = (char *)std::string("Dinamometr").c_str();
        this->IsAnalog = true;
        CommandCharacteristic = new BLECharacteristic("7dd71eaa-e6a6-4ecc-9296-39b144ca02af", BLERead | BLEWrite, ANALOG_COMMAND_SIZE, true);
        NotifyCharacteristic = new BLECharacteristic("63d223de-f93c-4d5c-8ecb-743e7ab0e676", BLERead | BLENotify, 6, true);
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