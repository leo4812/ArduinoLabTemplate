#include <Arduino.h>
#include "BaseSensor.hpp"

class Termistor : public BaseSensor
{

public:
    Termistor()
    {
        this->Name = (char *)std::string("Termistor").c_str();
        this->IsAnalog = true;
        CommandCharacteristic = new BLECharacteristic("972f7de0-e9cb-4000-a466-c03da4b69dd0", BLERead | BLEWrite, ANALOG_COMMAND_SIZE, true);
        NotifyCharacteristic = new BLECharacteristic("a8ffefd0-ab5a-4345-8cc6-97cf4f6e6ecb", BLERead | BLENotify, 6, true);
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
        if (this->isSerialConnection)
        {
            String strHEX = buffToHex(&buffer[0], 6);

            String Val = "a8ffefd0-ab5a-4345-8cc6-97cf4f6e6ecb";
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