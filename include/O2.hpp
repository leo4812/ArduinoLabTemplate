#include <Arduino.h>
#include "BaseSensor.hpp"
#include <SPI.h>
#include <Wire.h>
#include "Adafruit_MAX31855.h"

#define MAXCS 10

class O2 : public BaseSensor
{

public:
    O2()
    {
        this->Name = (char *)std::string("O2").c_str();
        CommandCharacteristic = new BLECharacteristic("DDB86CC3-DFBA-4CD7-8231-6266E1240FF1", BLERead | BLEWrite, DIGITAL_COMMAND_SIZE, true);
        NotifyCharacteristic = new BLECharacteristic("C5DABE49-261A-4C5A-8184-495323B37C6D", BLERead | BLENotify, 9, true);
    }

private:
    Adafruit_MAX31855 *thermocouple = nullptr;

    void pre_loop()
    {
        thermocouple = new Adafruit_MAX31855(MAXCS);
        thermocouple->begin();
    }
    void post_loop() {}
    void loop()
    {
        float b = thermocouple->readCelsius();
        uint32_t a = b / 0.25;

        uint8_t buffer[9] = {
            0,
        };
        memcpy(&buffer[1], (uint8_t *)&a, sizeof(a));
        this->NotifyCharacteristic->writeValue(buffer, sizeof(buffer));
    }
};
