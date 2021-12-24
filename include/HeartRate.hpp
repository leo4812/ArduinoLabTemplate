#include <Arduino.h>
#include "BaseSensor.hpp"

class HeartRate : public BaseSensor
{

public:
    HeartRate()
    {
        this->Name = (char *)std::string("HeartRate").c_str();
        this->IsAnalog = true;
        CommandCharacteristic = new BLECharacteristic("2bd94029-0017-4c53-a3d3-6549f1e6eafb", BLERead | BLEWrite, ANALOG_COMMAND_SIZE, true);
        NotifyCharacteristic = new BLECharacteristic("e0ff84e8-4ecb-4c1e-81f9-31f9dee48a36", BLERead | BLENotify, 6, true);
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