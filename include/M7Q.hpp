#include <Arduino.h>
#include "BaseSensor.hpp"

class M7Q : public BaseSensor
{

public:
    M7Q()
    {
        this->Name =  (char*) "M7Q";
        this->IsAnalog = true;
        CommandCharacteristic = new BLECharacteristic("cd1f0556-69d6-423a-8bd5-085558693719", BLERead | BLEWrite, ANALOG_COMMAND_SIZE, true);
        NotifyCharacteristic = new BLECharacteristic("464a279b-0347-48cb-b419-892bb4a95de1", BLERead | BLENotify, 6, true);
    }

private:
    void pre_loop()
    {
        pinMode(this->AnalogPort, INPUT);
    }
    void post_loop() {}
    void loop()
    {
        uint16_t CO = analogRead(this->AnalogPort);
        uint8_t buffer[6] = { 0, };
        buffer[1] = this->AnalogPort == A0 ? 0x00 : 0x01;
        memcpy(&buffer[2], &CO, sizeof(CO));
        this->NotifyCharacteristic->writeValue(buffer, sizeof(buffer));
    }
};