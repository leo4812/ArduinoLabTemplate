#include <Arduino.h>
#include "BaseSensor.hpp"

class GSR : public BaseSensor
{

public:
    GSR()
    {
        this->Name = (char *)std::string("GSR").c_str();
        this->IsAnalog = true;
        CommandCharacteristic = new BLECharacteristic("721d66b4-c4f3-4b32-af86-4df9a289b5d6", BLERead | BLEWrite, ANALOG_COMMAND_SIZE, true);
        NotifyCharacteristic = new BLECharacteristic("c453aa0b-4230-42ca-b538-f15949d89fcf", BLERead | BLENotify, 6, true);
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