#include <Arduino.h>
#include "BaseSensor.hpp"

using namespace rtos;

float voidRed = 0;   // Глобальная переменная красного для мутнокола
float voidGreen = 0; // Глобальная переменная зеленого для мутнокола
float voidBlue = 0;  // Глобальная переменная синего для мутнокола
float voidIK = 0;    // Глобальная переменная ик для мутнокола

class MutnoKol : public BaseSensor
{
public:
    const String strCommandCharacteristic = "5ae54c0c-dc29-44e4-8289-f1dab542430b";
    const String strNotifyCharacteristic = "e2ede542-79ce-407e-ac66-80d5f39e04fb";
    const String delimiter = ";"; // символ который ищем

    bool doWork = true;

    Thread *runThread;

    MutnoKol()
    {
        this->Name = (char *)std::string("MutnoKol").c_str();
        CommandCharacteristic = new BLECharacteristic("5ae54c0c-dc29-44e4-8289-f1dab542430b", BLERead | BLEWrite, DIGITAL_COMMAND_SIZE, true);
        NotifyCharacteristic = new BLECharacteristic("e2ede542-79ce-407e-ac66-80d5f39e04fb", BLERead | BLENotify, 17, true);
    }

private:
    void execute()
    {
        while (doWork)
        {
            if (Serial.available())
            {
                String str = Serial.readString(); // Принимаем строку в порт и приравниваем ее к нашей строке str
                // int Kolvo = str.length();         // Общее количество символов в принятой строке

                int8_t delOne;  // Индекс разделителя между GUID и KodOshibki
                int8_t delTwo;  // Индекс разделителя между KodOshibki и Red
                int8_t delTree; // Индекс разделителя между Red и Green
                int8_t delFour; // Индекс разделителя между Green и Blue
                int8_t delFive; // Индекс разделителя между Blue и IK

                String GUID;
                String KodOshibki;
                String strRed;
                String strGreen;
                String strBlue;
                String strIK;

                delOne = str.indexOf(delimiter); // Нашли первый разделитель
                GUID = str.substring(0, delOne); // Записали в строку GUID отрезок от начала до первого разделителя

                if (GUID.equals(strNotifyCharacteristic)) // Если GUID верный
                {

                    delTwo = str.indexOf(delimiter, (delOne + 1));    // Нашли второй разделитель
                    KodOshibki = str.substring((delOne + 1), delTwo); // Записали в строку KodOshibki отрезок от delOne+1 до delTwo разделителя

                    delTree = str.indexOf(delimiter, (delTwo + 1)); // Нашли третий разделитель
                    strRed = str.substring((delTwo + 1), delTree);  // Записали в строку strRed отрезок от delTwo+1 до delTree разделителя

                    delFour = str.indexOf(delimiter, (delTree + 1));  // Нашли четвертый разделитель
                    strGreen = str.substring((delTree + 1), delFour); // Записали в строку strGreen отрезок от delTree+1 до delFour разделителя

                    delFive = str.indexOf(delimiter, (delFour + 1)); // Нашли пятый разделитель
                    strBlue = str.substring((delFour + 1), delFive); // Записали в строку strBlue отрезок от delFour+1 до delFive разделителя

                    strIK = str.substring(delFive + 1); // Записали в строку strIK отрезок от delFive+1 и до конца

                    if ((delFive > 0) && (delFive < 254)) // Если не нашли четверный разделитель delFive будет -1 (Либо если до него и не дошли то delFive будет 255)
                    {

                        voidRed = strRed.toFloat();     // Конвертируем строку
                        voidGreen = strGreen.toFloat(); // Конвертируем строку
                        voidBlue = strBlue.toFloat();   // Конвертируем строку
                        voidIK = strIK.toFloat();       // Конвертируем строку
                    }
                }
            }
        }
    }

    void pre_loop()
    {
        //Serial.begin(9600);

        doWork = true;

        runThread = new Thread();
        runThread->start(callback(this, &MutnoKol::execute));

        String Start;
        Start += strCommandCharacteristic;
        Start += delimiter;
        Start += 1;
        Start += delimiter;
        Start += 0;
        Start += delimiter;
        Start += 0;
        Start += delimiter;
        Start += 2;
        Serial.println(Start); // Команда на старт мутнокола
    }

    void post_loop()
    {
        String Stop;
        Stop += strCommandCharacteristic;
        Stop += delimiter;
        Stop += 0;
        Stop += delimiter;
        Stop += 0;
        Stop += delimiter;
        Stop += 0;
        Stop += delimiter;
        Stop += 2;
        Serial.println(Stop); // Команда на стоп мутнокола

        doWork = false;
        if (runThread != nullptr)
        {
            runThread = nullptr;
        }
    }

    void loop()
    {
        float Red = voidRed;
        float Green = voidGreen;
        float Blue = voidBlue;
        float IK = voidIK;

        uint8_t buffer[17] = {
            0,
        };
        memcpy(&buffer[1], (uint8_t *)&Red, 4);
        memcpy(&buffer[5], (uint8_t *)&Green, 4);
        memcpy(&buffer[9], (uint8_t *)&Blue, 4);
        memcpy(&buffer[13], (uint8_t *)&IK, 4);
        this->NotifyCharacteristic->writeValue(buffer, sizeof(buffer));
    }
};