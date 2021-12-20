#include <Arduino.h>
#include "BaseSensor.hpp"

class Pcl : public BaseSensor
{

public:
    Pcl()
    {
        this->Name = (char *)std::string("Pcl").c_str();
        this->IsAnalog = true;
        CommandCharacteristic = new BLECharacteristic("B02915A3-0A52-4276-9224-A965E3BCF690", BLERead | BLEWrite, ANALOG_COMMAND_SIZE, true);
        NotifyCharacteristic = new BLECharacteristic("A9DB1E58-3634-4A1E-9597-5FAD370E38C6", BLERead | BLENotify, 6, true);
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