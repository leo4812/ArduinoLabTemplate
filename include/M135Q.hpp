#include <Arduino.h>
#include "BaseSensor.hpp"
#include <TroykaMQ.h>



class M135Q : public BaseSensor
{

public:
    MQ135 *mq135 = nullptr;

    M135Q()
    {
    

        this->Name = (char *)std::string("M135Q").c_str();
        this->IsAnalog = true;
        CommandCharacteristic = new BLECharacteristic("9cd2be24-32c6-43d4-a91d-2097e3ea1459", BLERead | BLEWrite, ANALOG_COMMAND_SIZE, true);
        NotifyCharacteristic = new BLECharacteristic("faf4a198-44c3-4154-94f3-6730e2045fb4", BLERead | BLENotify, 8, true);
    }

private:
    void pre_loop()
    {
        this->mq135 = new MQ135(this->AnalogPort);

        mq135->calibrate();
    }
    void post_loop() {}
    void loop()
    {
        float co2 = mq135->readCO2();
        uint16_t value = analogRead(this->AnalogPort);

        uint8_t buffer[8] = {
            0,
        };

        buffer[1] = this->AnalogPort == A0 ? 0x00 : 0x01;

        memcpy(&buffer[2], (uint8_t *)&co2, 4);

        memcpy(&buffer[6], &value, 2);
        this->NotifyCharacteristic->writeValue(buffer, sizeof(buffer));
    }
};