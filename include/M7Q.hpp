#include <Arduino.h>
#include "BaseSensor.hpp"

class M7Q : public BaseSensor
{

public:
    M7Q()
    {
        this->Name = (char *)std::string("M7Q").c_str();
        this->IsAnalog = true;
        CommandCharacteristic = new BLECharacteristic("C0E294C1-029A-4421-AA13-DE0C7E16A21B", BLERead | BLEWrite, ANALOG_COMMAND_SIZE, true);
        NotifyCharacteristic = new BLECharacteristic("A93C4467-02F7-45AE-87E6-3BA799560BD5", BLERead | BLENotify, 6, true);
    }

private:
    void pre_loop()
    {
        pinMode(this->AnalogPort, INPUT);
    }
    void post_loop() {}
    void loop()
    {
        uint32_t CO = analogRead(this->AnalogPort);
        uint8_t buffer[6] = { 0, };
        buffer[1] = this->AnalogPort == A0 ? 0x00 : 0x01;
        memcpy(&buffer[2], &CO, sizeof(CO));
        this->NotifyCharacteristic->writeValue(buffer, sizeof(buffer));
    }
};