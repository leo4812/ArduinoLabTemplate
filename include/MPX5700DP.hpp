#include <Arduino.h>
#include "BaseSensor.hpp"

class MPX5700DP : public BaseSensor
{

public:
    MPX5700DP()
    {
        this->Name = (char *)std::string("MPX5700DP").c_str();
        this->IsAnalog = true;
        CommandCharacteristic = new BLECharacteristic("6C8B0BBC-8096-4A20-9051-1819B5001EFD", BLERead | BLEWrite, ANALOG_COMMAND_SIZE, true);
        NotifyCharacteristic = new BLECharacteristic("D22A68DB-3CC5-44FA-BDDE-45528B2A367D", BLERead | BLENotify, 4, true);
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