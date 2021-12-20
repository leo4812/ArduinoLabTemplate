#include <Arduino.h>
#include "BaseSensor.hpp"

class PNO3 : public BaseSensor
{

public:
    PNO3()
    {
        this->Name = (char *)std::string("PNO3").c_str();
        this->IsAnalog = true;
        CommandCharacteristic = new BLECharacteristic("CF47685F-09C6-40C9-A442-A48513E7690A", BLERead | BLEWrite, ANALOG_COMMAND_SIZE, true);
        NotifyCharacteristic = new BLECharacteristic("E5E6F644-B15F-4713-9D9E-9CEB6B9D328F", BLERead | BLENotify, 6, true);
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