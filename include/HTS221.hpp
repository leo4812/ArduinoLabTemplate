#include <Arduino.h>
#include "BaseSensor.hpp"
#include <Arduino_HTS221.h>

class HTS221 : public BaseSensor
{

public:
    HTS221()
    {
        this->Name = (char *)std::string("HTS221").c_str();
        CommandCharacteristic = new BLECharacteristic("03BF53D8-2F5E-45DB-9F52-9FD737BF9605", BLERead | BLEWrite, DIGITAL_COMMAND_SIZE, true);
        NotifyCharacteristic = new BLECharacteristic("7C41E7D8-EF7C-4B11-A401-49ACA64F5962", BLERead | BLENotify, 9, true);
    }

private:
    void pre_loop()
    {
        HTS.begin();
    }
    void post_loop() {}
    void loop()
    {
        float temp = HTS.readTemperature(),
              humid = HTS.readHumidity();

        uint8_t buffer[9] = {
            0,
        };
        memcpy(&buffer[1], (uint8_t *)&temp, 4);
        memcpy(&buffer[5], (uint8_t *)&humid, 4);
        this->NotifyCharacteristic->writeValue(buffer, sizeof(buffer));
    }
};