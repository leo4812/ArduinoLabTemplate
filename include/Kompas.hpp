#include <Arduino.h>
#include "BaseSensor.hpp"

#include <Arduino_LSM9DS1.h>

class Kompas : public BaseSensor
{

public:
    Kompas()
    {
        this->Name = (char *)std::string("Kompas").c_str();
        CommandCharacteristic = new BLECharacteristic("99881f83-c738-4601-a370-2a508d12f413", BLERead | BLEWrite, DIGITAL_COMMAND_SIZE, true);
        NotifyCharacteristic = new BLECharacteristic("06543ba4-c183-4970-a1b9-6b0dfea20afb", BLERead | BLENotify, 13, true);
    }

private:
    void pre_loop()
    {
        IMU.begin();
    }
    void post_loop() {}
    void loop()
    {
        float xm, ym, zm;

        IMU.readMagneticField(xm, ym, zm);

        uint8_t buffer[13] = {
            0,
        };
        buffer[0] = 0;
        memcpy(&buffer[1], (uint8_t *)&xm, 4);
        memcpy(&buffer[5], (uint8_t *)&ym, 4);
        memcpy(&buffer[9], (uint8_t *)&zm, 4);
        if (this->isSerialConnection)
        {
            String strHEX = buffToHex(&buffer[0], 13);

            String Val = "06543ba4-c183-4970-a1b9-6b0dfea20afb";
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