#include <Arduino.h>
#include "BaseSensor.hpp"

class SoilMoisture : public BaseSensor
{

public:
    SoilMoisture()
    {
        this->Name = (char *)std::string("SoilMoisture").c_str();
        this->IsAnalog = true;
        CommandCharacteristic = new BLECharacteristic("6A8484A0-6C33-4BC2-9EFE-0E4AAFA79355", BLERead | BLEWrite, ANALOG_COMMAND_SIZE, true);
        NotifyCharacteristic = new BLECharacteristic("20AF8F2E-2D2A-46DE-B97E-9ACE2789A529", BLERead | BLENotify, 6, true);
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

            String Val = "20AF8F2E-2D2A-46DE-B97E-9ACE2789A529";
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