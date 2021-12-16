/*

#include <Arduino.h>
#include "BaseSensor.hpp"
#include <SPI.h>
#include "Adafruit_MAX31855.h"

const uint8_t MAXCS = 10;

Adafruit_MAX31855 thermocouple(MAXCS);

class MAX31855 : public BaseSensor
{

public:
    MAX31855()
    {        
        this->Name = (char*) "MAX31855";
        CommandCharacteristic = new BLECharacteristic("2DAF3C9C-CABA-461C-9FBC-1839D6F4E5B9", BLERead | BLEWrite, DIGITAL_COMMAND_SIZE, true);
        NotifyCharacteristic = new BLECharacteristic("F449E6B7-FE1E-45FF-AEBB-DCFC914DEB42", BLERead | BLENotify, 5, true);        
    }

private:
    void pre_loop()
    {
        thermocouple.begin();
    }
    void post_loop() {}
    void loop()
    {        
        float c = thermocouple.readCelsius();
        uint8_t buffer[5] = {0,};
        memcpy(&buffer[1], (uint8_t *)&c, sizeof(c));
        this->NotifyCharacteristic->writeValue(buffer, sizeof(buffer));
    }
};
*/
