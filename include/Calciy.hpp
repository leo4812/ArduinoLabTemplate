#include <Arduino.h>
#include "BaseSensor.hpp"

class Calciy : public BaseSensor
{

public:
    Calciy()
    {
        this->Name = (char *)std::string("Calciy").c_str();
        this->IsAnalog = true;
        CommandCharacteristic = new BLECharacteristic("671c2d29-981d-4b84-860a-f470734aa305", BLERead | BLEWrite, ANALOG_COMMAND_SIZE, true);
        NotifyCharacteristic = new BLECharacteristic("57949ebb-5abb-4930-b1b0-ac38046c54e4", BLERead | BLENotify, 6, true);
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