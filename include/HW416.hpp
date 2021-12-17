#include <Arduino.h>
#include "BaseSensor.hpp"

class HW416 : public BaseSensor
{

public:
    HW416()
    {
        this->Name = (char *)std::string("HW416").c_str();
        this->IsAnalog = true;
        CommandCharacteristic = new BLECharacteristic("c54846d5-e435-4a99-95df-25a6ce44a20b", BLERead | BLEWrite, ANALOG_COMMAND_SIZE, true);
        NotifyCharacteristic = new BLECharacteristic("f1a0d5f2-2b89-4ad7-af07-124cc52de383", BLERead | BLENotify, 3, true);
    }

private:
    void pre_loop()
    {
        pinMode(this->AnalogPort, INPUT);
    }
    void post_loop() {}
    void loop()
    {
        uint8_t value = digitalRead(this->AnalogPort);
        uint8_t buffer[3] = { 0, };
        buffer[1] = this->AnalogPort == A0 ? 0x00 : 0x01;
        memcpy(&buffer[2], &value, sizeof(value));
        this->NotifyCharacteristic->writeValue(buffer, sizeof(buffer));
    }
};