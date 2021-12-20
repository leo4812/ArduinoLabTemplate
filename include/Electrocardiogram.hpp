#include <Arduino.h>
#include "BaseSensor.hpp"

class Electrocardiogram : public BaseSensor
{

public:
    Electrocardiogram()
    {
        this->Name = (char *)std::string("Electrocardiogram").c_str();
        this->IsAnalog = true;
        CommandCharacteristic = new BLECharacteristic("2bc06faa-73ba-44b0-b141-7f767fe9acb8", BLERead | BLEWrite, ANALOG_COMMAND_SIZE, true);
        NotifyCharacteristic = new BLECharacteristic("c1b94f9f-1589-4742-b281-c6b37ac2a201", BLERead | BLENotify, 6, true);
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