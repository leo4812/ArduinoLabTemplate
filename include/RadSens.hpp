#include <Arduino.h>
#include "BaseSensor.hpp"
#include <Wire.h>
#include "radSens1v2.h"

ClimateGuard_RadSens1v2 radSens(RS_DEFAULT_I2C_ADDRESS);

class RadSens : public BaseSensor
{

public:
    RadSens()
    {        
        this->Name = (char*) "RadSens";
        CommandCharacteristic = new BLECharacteristic("d390c7e8-8450-4c1c-9e04-c8ab5a1993cc", BLERead | BLEWrite, DIGITAL_COMMAND_SIZE, true);
        NotifyCharacteristic = new BLECharacteristic("27025955-40d4-49a7-b3ca-f0771f2a3627", BLERead | BLENotify, 5, true);        
    }

private:
    void pre_loop()
    {
        radSens.radSens_init();

        uint8_t sensorChipId = radSens.getChipId();

        uint8_t firmWareVer = radSens.getFirmwareVersion();

        uint16_t sensitivity = radSens.getSensitivity();

        radSens.setSensitivity(55);

        sensitivity = radSens.getSensitivity();

        radSens.setSensitivity(105);

        bool hvGeneratorState = radSens.getHVGeneratorState();

        radSens.setHVGeneratorState(false);

        hvGeneratorState = radSens.getHVGeneratorState();

        radSens.setHVGeneratorState(true);

        hvGeneratorState = radSens.getHVGeneratorState();

        bool ledState = radSens.getLedState();

        radSens.setLedState(false);

        ledState = radSens.getLedState();

        radSens.setLedState(true);

        ledState = radSens.getLedState();
    }
    void post_loop() {}
    void loop()
    {        
        float Rad = radSens.getRadIntensyStatic();
        uint8_t buffer[5] = {0,};
        memcpy(&buffer[1], (uint8_t *)&Rad, 4);
        this->NotifyCharacteristic->writeValue(buffer, sizeof(buffer));
    }
};