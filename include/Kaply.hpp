#include "BaseSensor.hpp"
#include "PulseCounter.hpp"
class Kaply : public BaseSensor, public PulseCounter
{
public:
    Kaply()
    {
        this->Name = (char *)std::string("Kaply").c_str();
        this->IsAnalog = true;
        CommandCharacteristic = new BLECharacteristic("ee5729b8-14b9-430b-8c60-3108138b59eb", BLERead | BLEWrite, ANALOG_COMMAND_SIZE, true);
        NotifyCharacteristic = new BLECharacteristic("3c16c583-bd2d-46be-b4a4-e325d53ee3b3", BLERead | BLENotify, 6, true);
    }

    void pre_loop()
    {
        this->enableInterrupt(this->AnalogPort, RISING, false);
    }

    void post_loop()
    {
        this->counter_total = 0;

        this->disableInterrupt();
    }

    void loop()
    {

        auto tt = this->counter_total;

        uint32_t count = tt / 2;

        //Serial.println(count);

        uint8_t buffer[6] = {
            0,
        };
        buffer[1] = this->AnalogPort == A0 ? 0x00 : 0x01;
        memcpy(&buffer[2], (uint8_t *)&count, 4);
        this->NotifyCharacteristic->writeValue(buffer, sizeof(buffer));


    }

private:

};