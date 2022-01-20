#include "BaseSensor.hpp"
#include "PulseCounter.hpp"
#include "RingAverage.h"

class RPM : public BaseSensor, public PulseCounter
{
public:
    uint32_t Average;

    RPM()
    {

        fil = new RingAverage<uint32_t, 10>();

        this->Name = (char *)std::string("RPM").c_str();
        this->IsAnalog = true;
        CommandCharacteristic = new BLECharacteristic("5eb27efa-6a30-43c3-884e-439b465745ed", BLERead | BLEWrite, ANALOG_COMMAND_SIZE, true);
        NotifyCharacteristic = new BLECharacteristic("b4550ed3-5e29-4c03-95c3-853d65105bf7", BLERead | BLENotify, 6, true);
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

        float rez = (avr / 24) * 60;

        uint32_t rpm = rez;

        uint8_t buffer[6] = {
            0,
        };
        buffer[1] = this->AnalogPort == A0 ? 0x00 : 0x01;
        memcpy(&buffer[2], (uint8_t *)&rpm, sizeof(rpm));
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