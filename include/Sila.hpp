#include <Arduino.h>
#include "BaseSensor.hpp"

#include "HX711.h"

class Sila : public BaseSensor
{

public:
    Sila()
    {
        this->Name = (char *)std::string("Sila").c_str();
        CommandCharacteristic = new BLECharacteristic("15fb2cc1-4a1c-441a-80a9-4970f3d793b0", BLERead | BLEWrite, DIGITAL_COMMAND_SIZE, true);
        NotifyCharacteristic = new BLECharacteristic("bd267419-88e9-4786-9d7e-cb5a757119e2", BLERead | BLENotify, 5, true);
    }

private:
#define DT A4  // Указываем номер вывода, к которому подключен вывод DT  датчика
#define SCK A5 // Указываем номер вывода, к которому подключен вывод SCK датчика

    HX711 scale; // создаём объект scale

    float calibration_factor = 0.50; // вводим калибровочный коэффициент
    float units;                      // задаём переменную для измерений в граммах
    float ounces;

    void pre_loop()
    {
        scale.begin(DT, SCK); // инициируем работу с датчиком
        scale.set_scale();    // выполняем измерение значения без калибровочного коэффициента
        scale.tare();         // сбрасываем значения веса на датчике в 0
        scale.set_scale(calibration_factor);
    }
    void post_loop() {}
    void loop()
    {

        for (int i = 0; i < 10; i++)
        {                                   // усредняем показания, считав значения датчика 10 раз
            units = +scale.get_units(), 10; // суммируем показания 10 замеров
        }
        units = units / 10; // усредняем показания, разделив сумму значений на 10
        ounces = units * 0.035274;

        float sila = ounces;

        uint8_t buffer[5] = {
            0,
        };
        memcpy(&buffer[1], (uint8_t *)&sila, sizeof(sila));
        this->NotifyCharacteristic->writeValue(buffer, sizeof(buffer));
    }
};