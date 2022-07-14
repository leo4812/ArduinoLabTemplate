#include <Arduino.h>
#include "BaseSensor.hpp"
#include <Wire.h>
#include <Adafruit_ADS1X15.h>

class PNO3 : public BaseSensor
{

public:
    Adafruit_ADS1115 ads; // Создание объекта ads для общения через него с модулем

    PNO3()
    {
        this->Name = (char *)std::string("PNO3").c_str();
        CommandCharacteristic = new BLECharacteristic("CF47685F-09C6-40C9-A442-A48513E7690A", BLERead | BLEWrite, DIGITAL_COMMAND_SIZE, true);
        NotifyCharacteristic = new BLECharacteristic("E5E6F644-B15F-4713-9D9E-9CEB6B9D328F", BLERead | BLENotify, 5, true);
    }

private:
    void pre_loop()
    {
        ads.setGain(GAIN_TWOTHIRDS);
        ads.begin();
    }
    void post_loop() {}
    void loop()
    {
        int32_t adc_01_voltage; // Переменная для хранения значения АЦП вольтметра (канал 0-1)

        adc_01_voltage = ads.readADC_Differential_0_1(); // Измеряем значение АЦП на канало 0-1

        uint8_t buffer[5] = {
            0,
        };
        buffer[0] = 0;
        memcpy(&buffer[1], (uint8_t *)&adc_01_voltage, sizeof(adc_01_voltage));
        if (this->isSerialConnection)
        {
            String strHEX = buffToHex(&buffer[0], 5);
            String Val = "E5E6F644-B15F-4713-9D9E-9CEB6B9D328F";
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