#include <Arduino.h>
#include "BaseSensor.hpp"
#include <Adafruit_ADS1X15.h>
#include <Wire.h>

class ADS1115volt : public BaseSensor
{

public:
    Adafruit_ADS1115 ads; // Создание объекта ads для общения через него с модулем

    bool ErrorADS1115volt = true;

    ADS1115volt()
    {
        this->Name = (char *)std::string("ADS1115volt").c_str();
        CommandCharacteristic = new BLECharacteristic("ef0b8658-2db7-4591-8659-a69357076b4e", BLERead | BLEWrite, DIGITAL_COMMAND_SIZE, true);
        NotifyCharacteristic = new BLECharacteristic("cc86ef9f-242f-444c-8dc0-798fde983d97", BLERead | BLENotify, 5, true);
    }

private:
    void pre_loop()
    {
        ads.setGain(GAIN_TWOTHIRDS);
        ads.setDataRate(RATE_ADS1115_8SPS);

        if (!ads.begin())
        {
            ErrorADS1115volt = false;
        }
        else
        {
            ErrorADS1115volt = true;
        }
    }
    void post_loop() {}
    void loop()
    {

        int16_t adc_01_voltage; // Переменная для хранения значения АЦП вольтметра (канал 0-1)

        adc_01_voltage = ads.readADC_Differential_0_1(); // Измеряем значение АЦП на канало 0-1

        float u = float(adc_01_voltage) * 0.5590 / 1000.0; // Расчёт напряжения

        uint8_t buffer[5] = {
            0,
        };

        if (ErrorADS1115volt == false)
        {
            buffer[0] = 1;
            // Serial.println("Датчик не подключен");
        }
        else
        {
            buffer[0] = 0;
            memcpy(&buffer[1], (uint8_t *)&u, sizeof(u));

            //   Serial.print("VOLTAGE= ");
            //   Serial.print(u);
            //   Serial.println(" V");
        }
        if (flagSerial == true)
        {
            String strHEX = buffToHex(&buffer[0], 5);

            String Val = "cc86ef9f-242f-444c-8dc0-798fde983d97";
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