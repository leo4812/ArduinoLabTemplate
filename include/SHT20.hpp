#include <Arduino.h>
#include "BaseSensor.hpp"
#include "uFire_SHT20.h"

// uFire_SHT20 sht20;

class SHT20 : public BaseSensor
{

public:
    SHT20()
    {
        this->Name = (char *)std::string("SHT20").c_str();
        CommandCharacteristic = new BLECharacteristic("98a1190c-ecc3-46b5-a0fe-ed89968eded3", BLERead | BLEWrite, DIGITAL_COMMAND_SIZE, true);
        NotifyCharacteristic = new BLECharacteristic("fab12fa3-f2ad-4fa4-b79a-b41af0d1abe6", BLERead | BLENotify, 9, true);
    }

private:
    uFire_SHT20 sht20;

    void pre_loop()
    {
        Wire.begin();
        sht20.begin();
    }
    void post_loop() {}
    void loop()
    {
        float temp_c = sht20.temperature();
        float hum = sht20.humidity();

        uint8_t buffer[9] = {
            0,
        };

        if ((hum) < 0)
        {
            buffer[0] = 1;
        }
        else
        {

            memcpy(&buffer[1], (uint8_t *)&temp_c, 4);
            memcpy(&buffer[5], (uint8_t *)&hum, 4);
        }
        this->NotifyCharacteristic->writeValue(buffer, sizeof(buffer));

        // Serial.print("Темп: ");
        // Serial.print(temp_c);
        // Serial.print("    Влаж: ");
        // Serial.println(hum);
    }
};