#include <Arduino.h>
#include "BaseSensor.hpp"
#include <TroykaMQ.h>

int PIN_MQ135 = A0;           //////////////////////////////////        Так  вообще может работать?????      //////////////////////////////////

MQ135 mq135(PIN_MQ135);           ////////////////////////////            (Это единственный вариант скомпилить)     /////////////////////////////

// MQ135 mq135 (this->AnalogPort);      ///////////////////////     (ругается на this->)  который не может быть объявлен в этой области   /////

class M135Q : public BaseSensor
{
    // MQ135 mq135 (this->AnalogPort);    ///////////////////         ругается на необъявленный mq135    /////////////////////////

public:
    M135Q()
    {
    
        // MQ135 mq135(this->AnalogPort);      //////////////////////// ругается на необъявленный mq135      ///////////////////////

        this->Name = (char *)"M135Q";
        this->IsAnalog = true;
        CommandCharacteristic = new BLECharacteristic("9cd2be24-32c6-43d4-a91d-2097e3ea1459", BLERead | BLEWrite, ANALOG_COMMAND_SIZE, true);
        NotifyCharacteristic = new BLECharacteristic("faf4a198-44c3-4154-94f3-6730e2045fb4", BLERead | BLENotify, 8, true);
    }

private:
    void pre_loop()
    {
        PIN_MQ135 = this->AnalogPort;  /////////////////    по схеме от Беликова А0 у нас вроде как свободен    //////////////////////////

        pinMode(this->AnalogPort, INPUT);

        // MQ135 mq135(this->AnalogPort);   ////////////////////////////     ругается на необъявленный mq135       ////////////////////////////////

        mq135.calibrate();
    }
    void post_loop() {}
    void loop()
    {
        float co2 = mq135.readCO2();
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