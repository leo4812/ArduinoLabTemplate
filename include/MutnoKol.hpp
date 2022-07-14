#include <Arduino.h>
#include "BaseSensor.hpp"

using namespace rtos;

String strMutnokol;

class MutnoKol : public BaseSensor
{
public:
    uint8_t buffer[17] = {
        0,
    };

    void hexCharacterStringToBytes(byte *byteArray, const char *hexString)
    {
        bool oddLength = strlen(hexString) & 1;

        byte currentByte = 0;
        byte byteIndex = 0;

        for (byte charIndex = 0; charIndex < strlen(hexString); charIndex++)
        {
            bool oddCharIndex = charIndex & 1;

            if (oddLength)
            {
                // If the length is odd
                if (oddCharIndex)
                {
                    // odd characters go in high nibble
                    currentByte = nibble(hexString[charIndex]) << 4;
                }
                else
                {
                    // Even characters go into low nibble
                    currentByte |= nibble(hexString[charIndex]);
                    byteArray[byteIndex++] = currentByte;
                    currentByte = 0;
                }
            }
            else
            {
                // If the length is even
                if (!oddCharIndex)
                {
                    // Odd characters go into the high nibble
                    currentByte = nibble(hexString[charIndex]) << 4;
                }
                else
                {
                    // Odd characters go into low nibble
                    currentByte |= nibble(hexString[charIndex]);
                    byteArray[byteIndex++] = currentByte;
                    currentByte = 0;
                }
            }
        }
    }

    byte nibble(char c)
    {
        if (c >= '0' && c <= '9')
            return c - '0';

        if (c >= 'a' && c <= 'f')
            return c - 'a' + 10;

        if (c >= 'A' && c <= 'F')
            return c - 'A' + 10;

        return 0; // Not a valid hexadecimal character
    }

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
            if (Serial1.available())
            {
                strMutnokol = Serial1.readString(); // Принимаем строку в порт и приравниваем ее к нашей глобальной строке strMutnokol

                int8_t delOne; // Индекс разделителя между GUID и Data

                String GUID;
                String Data;

                delOne = strMutnokol.indexOf(delimiter); // Нашли первый разделитель
                GUID = strMutnokol.substring(0, delOne); // Записали в строку GUID отрезок от начала до первого разделителя

                if (GUID.equals(strNotifyCharacteristic)) // Если GUID верный
                {
                    Data = strMutnokol.substring(delOne + 1); // Записали в строку Data отрезок от delFive+1 и до конца
                    hexCharacterStringToBytes(buffer, Data.c_str());
                }
            }
        }
    }

    void pre_loop()
    {
        Serial1.begin(115200);

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
        Serial1.println(Start); // Команда на старт мутнокола
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
        Serial1.println(Stop); // Команда на стоп мутнокола

        doWork = false;
        if (runThread != nullptr)
        {
            runThread = nullptr;
        }
    }

    void loop()
    {
        if (this->isSerialConnection)
        {
            Serial.println(strMutnokol);
        }
        else
        {
            this->NotifyCharacteristic->writeValue(buffer, sizeof(buffer));
        }
    }
};