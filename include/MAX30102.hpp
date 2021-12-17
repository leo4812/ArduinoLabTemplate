#include <Arduino.h>
#include "BaseSensor.hpp"

#include "Wire.h"      //  Подключаем библиотеку для работы с шиной I2C
#include "MAX30105.h"  //  Подключаем библиотеку для работы с модулем
#include "heartRate.h" //  Подключаем блок для работы с ЧСС (пульс)

// const byte RATE_SIZE = 10; //  Коэффициент усреднения. ЧЕм больше число, тем больше усреднение показаний.

// bool ErrorMAX30102 = false; //Датчик не подключен

class MAX30102 : public BaseSensor
{

public:
    MAX30102()
    {

        this->Name = (char *)std::string("MAX30102").c_str();
        CommandCharacteristic = new BLECharacteristic("B980FFC1-D3A0-467F-89E2-EF26122D9B7C", BLERead | BLEWrite, DIGITAL_COMMAND_SIZE, true);
        NotifyCharacteristic = new BLECharacteristic("BFEF9C45-1DFA-4EE1-991C-2CFF1B15FB26", BLERead | BLENotify, 5, true);
    }

private:
    bool ErrorMAX30102 = false; //Датчик не подключен

    MAX30105 PARTICLE_SENSOR; //  Создаём объект для работы с библиотекой

    byte rates[10];       //  Массив со значениями ЧСС
    byte rateSpot = 0;    //  Переменная с порядковым номером значения в массиве
    long lastBeat = 0;    //  Время последнего зафиксированного удара
    float beatsPerMinute; //  Создаём переменную для хранения значения ЧСС
    int beatAvg;          //  Создаём переменную для хранения усреднённого значения ЧСС

    void pre_loop()
    {

        if (!PARTICLE_SENSOR.begin())
        {                         //  Инициируем работу с модулем. Если инициализация не прошла, то
            ErrorMAX30102 = true; //  выводим сообщение об этом в монитор последовательного порта
        }
        else
        {
            ErrorMAX30102 = false;
        }

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
                rateSpot %= 10;                           //  Задаём порядковый номер значения в массиве, возвращая остаток от деления и присваивая его переменной rateSpot
                beatAvg = 0;                              //  Обнуляем переменную и
                for (byte x = 0; x < 10; x++)
                {                        //  в цикле выполняем усреднение значений (чем больше RATE_SIZE, тем сильнее усреднение)
                    beatAvg += rates[x]; //  путём сложения всех элементов массива
                }
                beatAvg /= 10; //  а затем деления всей суммы на коэффициент усреднения (на общее количество элементов в массиве)
            }
        }

        float BPM;

        Serial.print("  Отдает датчик: ");
        Serial.print(beatAvg);
        Serial.print("  неусредненное значение: ");
        Serial.println(beatsPerMinute);

        if (irValue < 50000)
        {
            BPM = 0;
            Serial.println("Ошибка   irValue < 50000");
        }
        else
        {
            BPM = beatAvg;
            Serial.print("Условие else");
            Serial.print("  Переменная: ");
            Serial.println(BPM);
        }

        uint8_t buffer[5] = {
            0,
        };

        if (ErrorMAX30102 == true)
        {
            buffer[0] = 1;
        }
        else
        {

            memcpy(&buffer[1], (uint8_t *)&BPM, sizeof(BPM));
        }
        this->NotifyCharacteristic->writeValue(buffer, sizeof(buffer));
    }
};