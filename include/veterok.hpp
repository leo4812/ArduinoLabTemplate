#include "BaseSensor.hpp"
#include "PulseCounter.hpp"
#include "RingAverage.h"

class veterok : public BaseSensor, public PulseCounter
{
public:
    uint32_t Average;

    veterok()
    {

        fil = new RingAverage<uint32_t, 10>();

        this->Name = (char *)std::string("veterok").c_str();
        this->IsAnalog = true;
        CommandCharacteristic = new BLECharacteristic("79dd77dd-02f1-4c7d-8da0-4e40d30da599", BLERead | BLEWrite, ANALOG_COMMAND_SIZE, true);
        NotifyCharacteristic = new BLECharacteristic("cd64c99b-1329-44e1-b780-5265c7f4b57e", BLERead | BLENotify, 6, true);
    }

    void pre_loop()
    {
        pinMode(this->AnalogPort, INPUT);
        this->enableInterrupt(this->AnalogPort, RISING, false);
    }

    void post_loop()
    {
        this->disableInterrupt();
    }

    void loop()
    {

        uint32_t pulsesCount = 0;
        uint32_t current = micros();
        uint32_t time = current - last;
        uint32_t period = this->pulsePeriod(time);

        if (period != 0)
        {
            pulsesCount = 1000000 / period;
        }

        auto Average = fil->filtered(pulsesCount);

        float avr = Average;
        float rez = avr / 8;

        uint8_t buffer[6] = {
            0,
        };
        buffer[1] = this->AnalogPort == A0 ? 0x00 : 0x01;
        memcpy(&buffer[2], (uint8_t *)&rez, sizeof(rez));
        this->NotifyCharacteristic->writeValue(buffer, sizeof(buffer));

        /*
                Serial.print("Период: ");
                Serial.print(period);
                Serial.print("   Количество импульсов: ");
                Serial.print(pulsesCount);

                Serial.print("   Метры/сек: ");
                Serial.println(rez);
        */
        last = current;
    }

private:
    RingAverage<uint32_t, 10> *fil;
    uint32_t last = 0;
};