#include <Arduino.h>
#include "BaseSensor.hpp"
#include <Arduino_LPS22HB.h>

class LPS22HB : public BaseSensor
{

public:
    LPS22HB()
    {
        this->Name = (char*) std::string("LPS22HB").c_str();
        CommandCharacteristic = new BLECharacteristic("0FBA45BE-2DFE-45FF-9F52-9FD737380605", BLERead | BLEWrite, DIGITAL_COMMAND_SIZE, true);
        NotifyCharacteristic = new BLECharacteristic("FBF457D8-AF8B-4A22-A60F-49AFA64FA962", BLERead | BLENotify, 5, true);        
    }

private:
    void pre_loop()
    {
        BARO.begin();
    }
    void post_loop() {}
    void loop()
    {        
        float pressure = BARO.readPressure();
        uint8_t buffer[5] = {0,};
        memcpy(&buffer[1], (uint8_t *)&pressure, 4);
        this->NotifyCharacteristic->writeValue(buffer, sizeof(buffer));
    }
};