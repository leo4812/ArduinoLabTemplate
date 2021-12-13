#include <Arduino.h>
#include "BaseSensor.hpp"

class TDS : public BaseSensor
{

public:
    TDS()
    {
        this->Name =  (char*) "TDS";
        this->IsAnalog = true;
        CommandCharacteristic = new BLECharacteristic("FE0BECBC-6B15-4BEB-89EA-7FE670BC75C9", BLERead | BLEWrite, ANALOG_COMMAND_SIZE, true);
        NotifyCharacteristic = new BLECharacteristic("89117508-0D7E-47CC-9688-09177F4E979B", BLERead | BLENotify, 6, true);
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