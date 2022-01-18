#include "BaseSensor.hpp"
#include "PulseCounter.hpp"
class YFS201 : public BaseSensor, public PulseCounter
{
public:
    YFS201()
    {
        this->Name = (char *)std::string("YFS201").c_str();
        this->IsAnalog = true;
        CommandCharacteristic = new BLECharacteristic("66BE7886-C8D5-47E5-AF15-3B8B81982EF3", BLERead | BLEWrite, ANALOG_COMMAND_SIZE, true);
        NotifyCharacteristic = new BLECharacteristic("A2754F3F-33E8-42FB-857E-E8861E06BFA8", BLERead | BLENotify, 14, true);
    }

    void pre_loop()
    {
        this->enableInterrupt(this->AnalogPort, CHANGE);
    }

    void post_loop()
    {
        this->disableInterrupt();
    }

    void loop()
    {
        uint32_t pulsesCount = 0;
        float litres = 0.0f;
        uint32_t current = micros();
        uint32_t time = current - last;
        uint32_t period = this->pulsePeriod(time) * 2;
        if (period != 0)
        {
            pulsesCount = 1000000 / period;
            litres = pulsesCount / 450.0;
            printf("Period: %lu. Pps: %lu. Litres: %.3f\n", period, pulsesCount, litres);
        }
        uint8_t buffer[14] = {
            0,
        };
        buffer[1] = this->AnalogPort == A0 ? 0x00 : 0x01;
        memcpy(&buffer[2], (uint8_t *)&period, 4);
        memcpy(&buffer[6], (uint8_t *)&pulsesCount, 4);
        memcpy(&buffer[10], (uint8_t *)&litres, 4);
        this->NotifyCharacteristic->writeValue(buffer, sizeof(buffer));


        last = current;
    }

private:
    uint32_t last = 0;
};