#include <Arduino.h>
#include "BaseSensor.hpp"

class voltage : public BaseSensor
{

public:
    voltage()
    {
        this->Name =  (char*) "voltage";
        this->IsAnalog = true;
        CommandCharacteristic = new BLECharacteristic("EB0C9C2F-CD83-4460-8AC5-33DDAB36A393", BLERead | BLEWrite, ANALOG_COMMAND_SIZE, true);
        NotifyCharacteristic = new BLECharacteristic("3C04F8A5-C302-468F-975B-E0EFD4FF2DD4", BLERead | BLENotify, 6, true);
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