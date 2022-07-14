#include <Arduino.h>
#include "BaseSensor.hpp"

#include <Arduino_LSM9DS1.h>

class Giro : public BaseSensor
{

public:
    Giro()
    {
        this->Name = (char *)std::string("Giro").c_str();
        CommandCharacteristic = new BLECharacteristic("632a9fd2-dada-49e0-8e75-039562036d18", BLERead | BLEWrite, DIGITAL_COMMAND_SIZE, true);
        NotifyCharacteristic = new BLECharacteristic("e6bb896a-505f-4034-954a-2f05ee772117", BLERead | BLENotify, 13, true);
    }

private:
    void pre_loop()
    {
        IMU.begin();
    }
    void post_loop() {}
    void loop()
    {
        float xg, yg, zg;

        IMU.readGyroscope(xg, yg, zg);

        uint8_t buffer[13] = {
            0,
        };
        buffer[0] = 0;
        memcpy(&buffer[1], (uint8_t *)&xg, 4);
        memcpy(&buffer[5], (uint8_t *)&yg, 4);
        memcpy(&buffer[9], (uint8_t *)&zg, 4);
        if (this->isSerialConnection)
        {
            String strHEX = buffToHex(&buffer[0], 13);

            String Val = "e6bb896a-505f-4034-954a-2f05ee772117";
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