#include <Arduino.h>
#include "BaseSensor.hpp"

#include "HX711.h"

class Vesy : public BaseSensor
{

public:
    Vesy()
    {
        this->Name = (char *)std::string("Vesy").c_str();
        CommandCharacteristic = new BLECharacteristic("70028b3b-e3f1-4557-89cb-b5149e730beb", BLERead | BLEWrite, DIGITAL_COMMAND_SIZE, true);
        NotifyCharacteristic = new BLECharacteristic("cb36a985-ca03-4238-ae67-140c47b607bf", BLERead | BLENotify, 5, true);
    }

private:
#define DT A4  // Указываем номер вывода, к которому подключен вывод DT  датчика
#define SCK A5 // Указываем номер вывода, к которому подключен вывод SCK датчика

    HX711 scale; // создаём объект scale

    float calibration_factor = 1.825; // вводим калибровочный коэффициент
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

        float vesy = ounces;
        uint8_t buffer[5] = {
            0,
        };
        memcpy(&buffer[1], (uint8_t *)&vesy, sizeof(vesy));
        this->NotifyCharacteristic->writeValue(buffer, sizeof(buffer));
    }
};