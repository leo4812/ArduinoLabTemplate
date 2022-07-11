#include <Arduino.h>
#include "BaseSensor.hpp"

class MPX5700DP : public BaseSensor
{

public:
    MPX5700DP()
    {
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

        uint8_t buffer[6] = {
            0,
        };
        buffer[0] = 0;
        buffer[1] = this->AnalogPort == A0 ? 0x00 : 0x01;
        memcpy(&buffer[2], &value, sizeof(value));
        if (flagSerial == true)
        {
            String strHEX = buffToHex(&buffer[0], 6);

            String Val = "D22A68DB-3CC5-44FA-BDDE-45528B2A367D";
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