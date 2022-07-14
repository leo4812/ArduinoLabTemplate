#include <Arduino.h>
#include "BaseSensor.hpp"

#include <Arduino_LSM9DS1.h>

class Uskor : public BaseSensor
{

public:
    Uskor()
    {
        this->Name = (char *)std::string("Uskor").c_str();
        CommandCharacteristic = new BLECharacteristic("ad06255e-4b7c-4de4-a7ea-57cfe0c9344f", BLERead | BLEWrite, DIGITAL_COMMAND_SIZE, true);
        NotifyCharacteristic = new BLECharacteristic("3b94eab6-6c4e-4d53-babc-09efd1442ee9", BLERead | BLENotify, 13, true);
    }

private:
    void pre_loop()
    {
        IMU.begin();
    }
    void post_loop() {}
    void loop()
    {
        float xa, ya, za;

        IMU.readAcceleration(xa, ya, za);

        uint8_t buffer[13] = {
            0,
        };
        buffer[0] = 0;
        memcpy(&buffer[1], (uint8_t *)&xa, 4);
        memcpy(&buffer[5], (uint8_t *)&ya, 4);
        memcpy(&buffer[9], (uint8_t *)&za, 4);
        if (this->isSerialConnection)
        {
            String strHEX = buffToHex(&buffer[0], 13);

            String Val = "3b94eab6-6c4e-4d53-babc-09efd1442ee9";
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