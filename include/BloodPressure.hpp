#include <Arduino.h>
#include "BaseSensor.hpp"

class BloodPressure : public BaseSensor
{

public:
    BloodPressure()
    {
        this->Name = (char *)std::string("BloodPressure").c_str();
        this->IsAnalog = true;
        CommandCharacteristic = new BLECharacteristic("bbc7c68e-44f0-4c31-8dce-e4d3275b59f0", BLERead | BLEWrite, ANALOG_COMMAND_SIZE, true);
        NotifyCharacteristic = new BLECharacteristic("ed6c1eb6-fa91-445a-880d-fcd0f6e3e8ed", BLERead | BLENotify, 4, true);
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