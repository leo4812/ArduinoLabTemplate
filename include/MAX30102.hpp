#include <Arduino.h>
#include "BaseSensor.hpp"

#include "Wire.h"      //  Подключаем библиотеку для работы с шиной I2C
#include "MAX30105.h"  //  Подключаем библиотеку для работы с модулем
#include "heartRate.h" //  Подключаем блок для работы с ЧСС (пульс)

MAX30105 PARTICLE_SENSOR; //  Создаём объект для работы с библиотекой

const byte RATE_SIZE = 4; //  Коэффициент усреднения. ЧЕм больше число, тем больше усреднение показаний.
byte rates[RATE_SIZE];    //  Массив со значениями ЧСС
byte rateSpot = 0;        //  Переменная с порядковым номером значения в массиве
long lastBeat = 0;        //  Время последнего зафиксированного удара
float beatsPerMinute;     //  Создаём переменную для хранения значения ЧСС
int beatAvg;              //  Создаём переменную для хранения усреднённого значения ЧСС

class MAX30102 : public BaseSensor
{

public:
    MAX30102()
    {
        this->Name = (char *)"MAX30102";
        CommandCharacteristic = new BLECharacteristic("9abe01bf-55d3-4ae2-bc09-edb1a6c209c7", BLERead | BLEWrite, DIGITAL_COMMAND_SIZE, true);
        NotifyCharacteristic = new BLECharacteristic("ccf0279b-f3c5-4366-8b65-5dfd2d6741f8", BLERead | BLENotify, 5, true);
    }

private:
    void pre_loop()
    {
        PARTICLE_SENSOR.begin();

        PARTICLE_SENSOR.setup();                    //  Устанавливаем настройки для сенсора по умолчанию
        PARTICLE_SENSOR.setPulseAmplitudeRed(0x0A); //  Выключаем КРАСНЫЙ светодиод для того, чтобы модуль начал работу
        PARTICLE_SENSOR.setPulseAmplitudeGreen(0);  //  Выключаем ЗЕЛЁНЫЙ светодиод
    }
    void post_loop() {}
    void loop()
    {

        long irValue = PARTICLE_SENSOR.getIR(); //  Считываем значение отражённого ИК-светодиода (отвечающего за пульс) и
        if (checkForBeat(irValue) == true)
        {                                           //  если пульс был зафиксирован, то
            long delta = millis() - lastBeat;       //  находим дельту по времени между ударами
            lastBeat = millis();                    //  Обновляем счётчик
            beatsPerMinute = 60 / (delta / 1000.0); //  Вычисляем количество ударов в минуту
            if (beatsPerMinute < 255 && beatsPerMinute > 20)
            {                                             //  Если количество ударов в минуту находится в промежутке между 20 и 255, то
                rates[rateSpot++] = (byte)beatsPerMinute; //  записываем это значение в массив значений ЧСС
                rateSpot %= RATE_SIZE;                    //  Задаём порядковый номер значения в массиве, возвращая остаток от деления и присваивая его переменной rateSpot
                beatAvg = 0;                              //  Обнуляем переменную и
                for (byte x = 0; x < RATE_SIZE; x++)
                {                        //  в цикле выполняем усреднение значений (чем больше RATE_SIZE, тем сильнее усреднение)
                    beatAvg += rates[x]; //  путём сложения всех элементов массива
                }
                beatAvg /= RATE_SIZE; //  а затем деления всей суммы на коэффициент усреднения (на общее количество элементов в массиве)
            }
        }

        float BPM;

        if (irValue < 50000)
        {
            BPM = 0;
        }
        else
        {
            BPM = beatAvg;
        }

        uint8_t buffer[5] = {
            0,
        };
        memcpy(&buffer[1], (uint8_t *)&BPM, 4);
        this->NotifyCharacteristic->writeValue(buffer, sizeof(buffer));
    }
};