#include <Arduino.h>
#include "BaseSensor.hpp"
#include "RingAverage.h"

class MPX5700DP : public BaseSensor
{

public:
    uint32_t PressureFILTR;

    MPX5700DP()
    {
        fil = new RingAverage<uint32_t, 10>();

        this->Name = (char *)std::string("MPX5700DP").c_str();
        this->IsAnalog = true;
        CommandCharacteristic = new BLECharacteristic("6C8B0BBC-8096-4A20-9051-1819B5001EFD", BLERead | BLEWrite, ANALOG_COMMAND_SIZE, true);
        NotifyCharacteristic = new BLECharacteristic("D22A68DB-3CC5-44FA-BDDE-45528B2A367D", BLERead | BLENotify, 6, true);
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

        auto PressureFILTR = fil->filtered(value);

        uint8_t buffer[6] = {
            0,
        };
        buffer[1] = this->AnalogPort == A0 ? 0x00 : 0x01;
        memcpy(&buffer[2], &PressureFILTR, sizeof(PressureFILTR));
        this->NotifyCharacteristic->writeValue(buffer, sizeof(buffer));
    }
    RingAverage<uint32_t, 10> *fil;
};