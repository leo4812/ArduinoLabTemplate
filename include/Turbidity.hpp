#include <Arduino.h>
#include "BaseSensor.hpp"

class Turbidity : public BaseSensor
{

public:
    Turbidity()
    {
        this->Name = (char *)std::string("Turbidity").c_str();
        this->IsAnalog = true;
        CommandCharacteristic = new BLECharacteristic("9F92BB52-949B-42FA-AD42-AA735CCAFB70", BLERead | BLEWrite, ANALOG_COMMAND_SIZE, true);
        NotifyCharacteristic = new BLECharacteristic("9B64F067-E2A7-4F81-8355-BB71B006E795", BLERead | BLENotify, 4, true);
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
        uint8_t buffer[4] = { 0, };
        buffer[1] = this->AnalogPort == A0 ? 0x00 : 0x01;
        memcpy(&buffer[2], &value, sizeof(value));
        this->NotifyCharacteristic->writeValue(buffer, sizeof(buffer));
    }
};