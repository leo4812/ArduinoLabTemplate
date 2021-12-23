#include <Arduino.h>
#include "BaseSensor.hpp"

#include <Wire.h>
#include <Adafruit_BMP085.h>

class BMP180 : public BaseSensor
{

public:
    BMP180()
    {        
        this->Name = (char *)std::string("BMP180").c_str();
        CommandCharacteristic = new BLECharacteristic("fd7c04e8-b815-432f-b681-1693be4124ab", BLERead | BLEWrite, DIGITAL_COMMAND_SIZE, true);
        NotifyCharacteristic = new BLECharacteristic("f69ce11b-8410-4498-96f1-c18b5941c6ef", BLERead | BLENotify, 5, true);        
    }

private:

Adafruit_BMP085 bmp;

    void pre_loop()
    {
        bmp.begin();
    }
    void post_loop() {}
    void loop()
    {        
        float pressure180 = bmp.readPressure();
        uint8_t buffer[5] = {0,};
        memcpy(&buffer[1], (uint8_t *)&pressure180, sizeof(pressure180));
        this->NotifyCharacteristic->writeValue(buffer, sizeof(buffer));
    }
};