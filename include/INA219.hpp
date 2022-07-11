#include <Arduino.h>
#include "BaseSensor.hpp"
#include <Wire.h>
#include <Adafruit_INA219.h>

class INA219 : public BaseSensor
{

public:
    Adafruit_INA219 ina219;

    bool ErrorINA219 = true;

    INA219()
    {
        this->Name = (char *)std::string("INA219").c_str();
        CommandCharacteristic = new BLECharacteristic("d973de6e-67a7-4c24-9640-a40435cf1fbc", BLERead | BLEWrite, DIGITAL_COMMAND_SIZE, true);
        NotifyCharacteristic = new BLECharacteristic("38c20915-7132-4e1b-9664-3ada1ea5be86", BLERead | BLENotify, 5, true);
    }

private:
    void pre_loop()
    {
        if (!ina219.begin())
        {
            ErrorINA219 = false;
        }
    }
    void post_loop() {}
    void loop()
    {
        int32_t current_mA = 0;

        float current_A = 0;

        if (ina219.getCurrent_mA() < 0)
        {
            current_mA = ina219.getCurrent_mA() * -1;
        }
        else
        {
            current_mA = ina219.getCurrent_mA();
        }

        float PER = current_mA;

        current_A = PER / 1000;

        uint8_t buffer[5] = {
            0,
        };

        if (ErrorINA219 == false)
        {

            buffer[0] = 1;
            // Serial.println("Датчик не подключен");
        }
        else
        {
            buffer[0] = 0;
            memcpy(&buffer[1], (uint8_t *)&current_A, sizeof(current_A));
            // Serial.print("ТОК :  ");
            // Serial.print(current_mA);
            // Serial.println(" mA");
        }
        if (flagSerial == true)
        {
            String strHEX = buffToHex(&buffer[0], 5);

            String Val = "38c20915-7132-4e1b-9664-3ada1ea5be86";
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