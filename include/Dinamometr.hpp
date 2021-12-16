#include <Arduino.h>
#include "BaseSensor.hpp"

class Dinamometr : public BaseSensor
{

public:
    Dinamometr()
    {
        this->Name =  (char*) "Dinamometr";
        this->IsAnalog = true;
        CommandCharacteristic = new BLECharacteristic("00ca339b-0bd0-4cda-b230-4a9857ecfeca", BLERead | BLEWrite, ANALOG_COMMAND_SIZE, true);
        NotifyCharacteristic = new BLECharacteristic("c4d4418f-1692-4a14-ba52-552ee14b142f", BLERead | BLENotify, 6, true);
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