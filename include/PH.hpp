#include <Arduino.h>
#include "BaseSensor.hpp"

class PH : public BaseSensor
{

public:
    PH()
    {
        this->Name =  (char*) "PH";
        this->IsAnalog = true;
        CommandCharacteristic = new BLECharacteristic("4572926D-B013-4868-9DD1-A930CD44D7FF", BLERead | BLEWrite, ANALOG_COMMAND_SIZE, true);
        NotifyCharacteristic = new BLECharacteristic("9B264AE2-98F1-4905-A495-C15113A0D35B", BLERead | BLENotify, 6, true);
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
        uint8_t buffer[6] = { 0, };
        buffer[1] = this->AnalogPort == A0 ? 0x00 : 0x01;
        memcpy(&buffer[2], &value, sizeof(value));
        this->NotifyCharacteristic->writeValue(buffer, sizeof(buffer));
    }
};