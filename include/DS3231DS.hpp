#include <Arduino.h>
#include "BaseSensor.hpp"
#include <Wire.h>
#include <DS3231.h>

class DS3231DS : public BaseSensor
{

public:
    DS3231DS()
    {
        this->Name = (char *)std::string("DS3231DS").c_str();
        CommandCharacteristic = new BLECharacteristic("9c75fa05-191f-4e4b-a210-905c6310d4d7", BLERead | BLEWrite, DIGITAL_COMMAND_SIZE, true);
        NotifyCharacteristic = new BLECharacteristic("d8f6f104-69e5-4cfd-93a6-8815b4e04d88", BLERead | BLENotify, 13, true);
    }

private:
    RTClib myRTC;

    void pre_loop()
    {
        Wire.begin();
    }
    void post_loop() {}
    void loop()
    {
        DateTime now = myRTC.now();

        uint16_t year = now.year();
        uint16_t month = now.month();
        uint16_t day = now.day();
        uint16_t hour = now.hour();
        uint16_t minute = now.minute();
        uint16_t second = now.second();

        uint8_t buffer[13] = {
            0,
        };

        memcpy(&buffer[1], (uint8_t *)&year, 2);
        memcpy(&buffer[3], (uint8_t *)&month, 2);
        memcpy(&buffer[5], (uint8_t *)&day, 2);
        memcpy(&buffer[7], (uint8_t *)&hour, 2);
        memcpy(&buffer[9], (uint8_t *)&minute, 2);
        memcpy(&buffer[11], (uint8_t *)&second, 2);

        this->NotifyCharacteristic->writeValue(buffer, sizeof(buffer));
        /*
                Serial.print("Год: ");
                Serial.print(year);
                Serial.print("  Месяц: ");
                Serial.print(month);
                Serial.print("  День: ");
                Serial.print(day);
                Serial.print("  Часы: ");
                Serial.print(hour);
                Serial.print("  Минуты: ");
                Serial.print(minute);
                Serial.print("  Секунды: ");
                Serial.println(second);
                */
    }
};