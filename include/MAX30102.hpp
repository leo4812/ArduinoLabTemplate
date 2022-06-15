#include <Arduino.h>
#include "BaseSensor.hpp"

#include "Wire.h"      //  Подключаем библиотеку для работы с шиной I2C
#include "MAX30105.h"  //  Подключаем библиотеку для работы с модулем
#include "heartRate.h" //  Подключаем блок для работы с ЧСС (пульс)

using namespace rtos;

class MAX30102 : public BaseSensor
{

public:
    MAX30105 particleSensor;

    long unblockedValue;
    long lastBeat = 0;
    float beatsPerMinute;
    int beatAvg;
    // const byte RATE_SIZE = 6;    // Increase this for more averaging. 4 is good.
    byte rates[6]; // Array of heart rates
    byte rateSpot = 0;

    volatile uint32_t ir = 0;

    volatile uint32_t sinus = 0; // Переменная синусойды
    volatile uint8_t BPM = 0;    // Переменная усредненного количества ЧСС

    Thread *runThread;
    Thread *bpmThread;

    bool doWork = true;

    MAX30102()
    {

        this->Name = (char *)std::string("MAX30102").c_str();
        CommandCharacteristic = new BLECharacteristic("9abe01bf-55d3-4ae2-bc09-edb1a6c209c7", BLERead | BLEWrite, DIGITAL_COMMAND_SIZE, true);
        NotifyCharacteristic = new BLECharacteristic("ccf0279b-f3c5-4366-8b65-5dfd2d6741f8", BLERead | BLENotify, 22, true);
    }

private:
    void execute()
    {
        while (doWork)
        {

            /*
                        if (checkForBeat(ir) == true)
                        {
                            long delta = millis() - lastBeat;
                            lastBeat = millis();
                            beatsPerMinute = 60 / (delta / 1000.0);
                            if (beatsPerMinute < 255 && beatsPerMinute > 20)
                            {
                                rates[rateSpot++] = (byte)beatsPerMinute; // Store this reading in the array
                                rateSpot %= 6;                    // Wrap variable

                                // Take average of readings
                                beatAvg = 0;
                                for (byte x = 0; x < 6; x++)
                                    beatAvg += rates[x];
                                beatAvg /= 6;
                            }
                        }
            */

            uint32_t ValueArray[5] = {
                0,
            };
            for (int i = 0; i < 5; i++)
            {
                ir = particleSensor.getIR();
                if (ir < 50000)
                {
                    sinus = 0;
                    // BPM = 0;
                }
                else
                {
                    sinus = ir;
                    // BPM = beatAvg;
                }
                ValueArray[i] = sinus;
            }

            uint8_t buffer[22] = {
                0,
            };
            memcpy(&buffer[2], (uint8_t *)&ValueArray[0], 4);
            memcpy(&buffer[6], (uint8_t *)&ValueArray[1], 4);
            memcpy(&buffer[10], (uint8_t *)&ValueArray[2], 4);
            memcpy(&buffer[14], (uint8_t *)&ValueArray[3], 4);
            memcpy(&buffer[18], (uint8_t *)&ValueArray[4], 4);
            this->NotifyCharacteristic->writeValue(buffer, sizeof(buffer));
        }
    }

    void execute_bpm()
    {
        while (doWork)
        {
            // БЛА БЛА БЛА поток
        }
    }

    void pre_loop()
    {
        doWork = true;
        runThread = new Thread();
        runThread->start(callback(this, &MAX30102::execute));

        bpmThread = new Thread();
        bpmThread->start(callback(this, &MAX30102::execute_bpm));

        Serial.begin(9600);
        particleSensor.begin(Wire, I2C_SPEED_FAST);
        byte ledBrightness = 0x77;
        byte sampleAverage = 4;
        byte ledMode = 3;
        int sampleRate = 1000;
        int pulseWidth = 411;
        int adcRange = 16384;
        particleSensor.setup(ledBrightness, sampleAverage, ledMode, sampleRate, pulseWidth, adcRange);
        particleSensor.setPulseAmplitudeRed(0);
        particleSensor.setPulseAmplitudeGreen(0);

        unblockedValue = 0;
        for (byte x = 0; x < 32; x++)
        {
            unblockedValue += particleSensor.getIR();
        }
        unblockedValue /= 32;
    }
    void post_loop()
    {
        doWork = false;
        if (runThread != nullptr)
        {
            runThread = nullptr;
        }
        if (bpmThread != nullptr)
        {
            bpmThread = nullptr;
        }
    }
    void loop()
    {
    }
};