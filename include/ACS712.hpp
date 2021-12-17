#include <Arduino.h>
#include "BaseSensor.hpp"

class ACS712 : public BaseSensor
{

public:
    ACS712()
    {
        this->Name = (char *)std::string("ACS712").c_str();
        this->IsAnalog = true;
        CommandCharacteristic = new BLECharacteristic("9B69DE8C-B847-4F81-A3E6-9D6998740D15", BLERead | BLEWrite, ANALOG_COMMAND_SIZE, true);
        NotifyCharacteristic = new BLECharacteristic("A300379E-2934-43F7-BA6D-AECF4CF6605B", BLERead | BLENotify, 4, true);
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