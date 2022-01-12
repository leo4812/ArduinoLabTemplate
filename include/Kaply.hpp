#include <Arduino.h>
#include "BaseSensor.hpp"

class Kaply : public BaseSensor
{

public:
    Kaply()
    {
        this->Name = (char *)std::string("Kaply").c_str();
        this->IsAnalog = true;
        CommandCharacteristic = new BLECharacteristic("ee5729b8-14b9-430b-8c60-3108138b59eb", BLERead | BLEWrite, ANALOG_COMMAND_SIZE, true);
        NotifyCharacteristic = new BLECharacteristic("3c16c583-bd2d-46be-b4a4-e325d53ee3b3", BLERead | BLENotify, 6, true);
    }

private:
    uint32_t kaply = 0;

    bool flag = false;

    void pre_loop()
    {
        pinMode(this->AnalogPort, INPUT);
    }
    void post_loop() {}
    void loop()
    {
        if (digitalRead(this->AnalogPort) == 1)
        {

            if (((digitalRead(this->AnalogPort)) == 1) && (flag == false))
            {
                flag = true;
                kaply = kaply + 1;
            }
        }

        else if (((digitalRead(this->AnalogPort)) == 0) && (flag == true))
        {
            flag = false;
            thread_sleep_for(50);     // Вместо delay
        }

        else
        {

            flag = false;
        }

        //Serial.println(kaply);

        
        uint8_t buffer[6] = {
            0,
        };
        buffer[1] = this->AnalogPort == A0 ? 0x00 : 0x01;
        memcpy(&buffer[2], &kaply, sizeof(kaply));
        this->NotifyCharacteristic->writeValue(buffer, sizeof(buffer));
    }
};