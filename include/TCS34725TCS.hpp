#include <Arduino.h>
#include "BaseSensor.hpp"

#include <Wire.h>
#include "Adafruit_TCS34725.h"

class TCS34725TCS : public BaseSensor
{

public:
    TCS34725TCS()
    {
        this->Name = (char *)std::string("TCS34725TCS").c_str();
        CommandCharacteristic = new BLECharacteristic("6a8bf3e8-d096-4014-9962-ad2cd48e5cbd", BLERead | BLEWrite, DIGITAL_COMMAND_SIZE, true);
        NotifyCharacteristic = new BLECharacteristic("999593c0-8f6c-40c0-a23b-7a3ad793c219", BLERead | BLENotify, 13, true);
    }

private:
    Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_420MS, TCS34725_GAIN_1X);

    void pre_loop()
    {
        tcs.begin();
    }
    void post_loop() {}
    void loop()
    {
        float red, green, blue;

        uint8_t s[13] = {
            0,
        };

        tcs.getRGB(&red, &green, &blue);

        // Serial.print("Красный: ");
        // Serial.print(red);
        // Serial.print("  Зеленый: ");
        // Serial.print(green);
        // Serial.print("  Синий: ");
        // Serial.println(blue);

        memcpy(&s[1], (uint8_t *)&red, 4);
        memcpy(&s[5], (uint8_t *)&green, 4);
        memcpy(&s[9], (uint8_t *)&blue, 4);

        this->NotifyCharacteristic->writeValue(s, sizeof(s));
    }
};