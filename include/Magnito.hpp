#include <Arduino.h>
#include "BaseSensor.hpp"

#include <Arduino_LSM9DS1.h>

class Magnito : public BaseSensor
{

public:
    Magnito()
    {
        this->Name = (char *)std::string("Magnito").c_str();
        CommandCharacteristic = new BLECharacteristic("c98bec3c-e23d-4dea-9431-b615c345a064", BLERead | BLEWrite, DIGITAL_COMMAND_SIZE, true);
        NotifyCharacteristic = new BLECharacteristic("2ea0f8b8-1baf-418d-9ed0-f6ac0c8018d1", BLERead | BLENotify, 13, true);
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
        memcpy(&buffer[1], (uint8_t *)&xm, 4);
        memcpy(&buffer[5], (uint8_t *)&ym, 4);
        memcpy(&buffer[9], (uint8_t *)&zm, 4);
        this->NotifyCharacteristic->writeValue(buffer, sizeof(buffer));
    }
};