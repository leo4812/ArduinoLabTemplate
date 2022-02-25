#include <Arduino.h>
#include "BaseSensor.hpp"

#include <PDM.h>
#include <math.h>

using namespace rtos;

void g_onPDMdata();

void *instance;

class MP34DT05 : public BaseSensor
{
public:
    MP34DT05()
    {
        instance = this;
        this->Name = (char *)std::string("MP34DT05").c_str();
        CommandCharacteristic = new BLECharacteristic("fe583104-4186-40c1-b281-eb38a9536063", BLERead | BLEWrite, DIGITAL_COMMAND_SIZE, true);
        NotifyCharacteristic = new BLECharacteristic("3cd72a1b-ddb0-4a48-9716-83251663ddbf", BLERead | BLENotify, 5, true);
    }

    // static MP34DT05 *instance;
    double MP34DT05_AOP = 122.5;
    double MP34DT05_SNR = 64;
    double MP34DT05_dBFS = -26;
    char channels = 1;
    int frequency = 16000;

    Thread *runThread;
    bool doWork = true;
    short sampleBuffer[512];
    volatile int samplesRead;

    volatile float amplitude;

    mbed::Callback<void()> clb;

    void onPDMdata()
    {
        int bytesAvailable = PDM.available();
        PDM.read(sampleBuffer, bytesAvailable);

        samplesRead = bytesAvailable / 2;
    }

    void execute()
    {
        while (doWork)
        {
            if (samplesRead)
            {
                for (int i = 0; i < samplesRead; i++)
                {
                    short sample = sampleBuffer[i];
                    int sample_absolute = abs(sample);
                    if (sample_absolute != 0)
                    {
                        amplitude = 20 * log10(sample_absolute / MP34DT05_AOP) + MP34DT05_SNR;
                    }
                }
                samplesRead = 0;
            }
          
            Serial.print("   Поток работает    ");  //           ТУТ
            Serial.println(amplitude);  //                       ТУТ
        }
        Serial.print("Выход из цикла while  ");  //              ТУТ
        Serial.print("  Переменная в цикле ");  //               ТУТ
        Serial.println(doWork);  //                              ТУТ
    }

    void pre_loop()
    {
        this->clb = mbed::callback(this, &MP34DT05::onPDMdata);

        PDM.onReceive(g_onPDMdata);
        PDM.begin(channels, frequency);
        runThread = new Thread();
        runThread->start(callback(this, &MP34DT05::execute));

        Serial.print("Переменная вайла в прелупе:   ");  //      ТУТ
        Serial.println(doWork);  //                              ТУТ
    }

    void post_loop()
    {
        doWork = false;
        runThread->join();
        delete runThread;
        Serial.print("Переменная вайла в ПОСТЛУП:  ");  //                        ТУТ
        Serial.println(doWork);  //                          ТУТ
    }

    void loop()
    {
        if (!isinf(amplitude))
        {
            //printf("$%.2f;\n", amplitude);
            
            uint8_t buffer[5] = {
                0,
            };
            memcpy(&buffer[1], (uint8_t *)&amplitude, sizeof(amplitude));
            this->NotifyCharacteristic->writeValue(buffer, sizeof(buffer));
        }
    }
};

void g_onPDMdata()
{
    MP34DT05* m = static_cast<MP34DT05*>(instance);
    m->onPDMdata();
}
