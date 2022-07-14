#include <Arduino.h>
#include "BaseSensor.hpp"
#include <Wire.h>
#include <Adafruit_ADS1X15.h>

class Pcl : public BaseSensor
{

public:
    Adafruit_ADS1115 ads; // Создание объекта ads для общения через него с модулем

    Pcl()
    {
        this->Name = (char *)std::string("Pcl").c_str();
        CommandCharacteristic = new BLECharacteristic("B02915A3-0A52-4276-9224-A965E3BCF690", BLERead | BLEWrite, DIGITAL_COMMAND_SIZE, true);
        NotifyCharacteristic = new BLECharacteristic("A9DB1E58-3634-4A1E-9597-5FAD370E38C6", BLERead | BLENotify, 5, true);
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
            String Val = "A9DB1E58-3634-4A1E-9597-5FAD370E38C6";
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