#include <Arduino.h>
#include "BaseSensor.hpp"

class datUF : public BaseSensor
{

public:
    datUF()
    {
        this->Name = (char *)std::string("datUF").c_str();
        this->IsAnalog = true;
        CommandCharacteristic = new BLECharacteristic("cf5d66bc-a8f0-484c-80dd-a1e04ec5a598", BLERead | BLEWrite, ANALOG_COMMAND_SIZE, true);
        NotifyCharacteristic = new BLECharacteristic("5ea82d4b-2cc0-47b3-8add-1af18ffd2c9f", BLERead | BLENotify, 4, true);
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