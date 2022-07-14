#include <Arduino.h>
#include "BaseSensor.hpp"

class MagnetPole : public BaseSensor
{

public:
    MagnetPole()
    {
        this->Name = (char *)std::string("MagnetPole").c_str();
        this->IsAnalog = true;
        CommandCharacteristic = new BLECharacteristic("3B75281E-00A0-4424-84C5-4C549CC1AE82", BLERead | BLEWrite, ANALOG_COMMAND_SIZE, true);
        NotifyCharacteristic = new BLECharacteristic("EF8A1B0B-1005-4DAD-B49D-75F84488E52C", BLERead | BLENotify, 6, true);
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

        uint8_t buffer[6]{
            0,
        };
        buffer[0] = 0;
        buffer[1] = this->AnalogPort == A0 ? 0x00 : 0x01;
        memcpy(&buffer[2], (uint8_t *)&value, 4);

        if (this->isSerialConnection)
        {
            String strHEX = buffToHex(&buffer[0], 6);

            String Val = "EF8A1B0B-1005-4DAD-B49D-75F84488E52C";
            Val += ";";
            Val += strHEX;
            Serial.println(Val);
        }
        else
        {
            this->NotifyCharacteristic->writeValue(buffer, sizeof(buffer));
        }
    }
};