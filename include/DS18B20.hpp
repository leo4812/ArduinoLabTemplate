#include <Arduino.h>
#include "BaseSensor.hpp"
#define MAXIMWIRE_EXTERNAL_PULLUP
#include <MaximWire.h>


MaximWire::Bus line(7);
MaximWire::DS18B20 dallas;

class DS18B20 : public BaseSensor
{

public:
    DS18B20()
    {        
        this->Name = (char*) "DS18B20";
        CommandCharacteristic = new BLECharacteristic("09EB425D-8627-4FCB-AEA8-638BCF3F73F7", BLERead | BLEWrite, DIGITAL_COMMAND_SIZE, true);
        NotifyCharacteristic = new BLECharacteristic("8103F9B3-C91E-47CD-8634-4B7D8F4D018D", BLERead | BLENotify, 5, true);        
    }

private:
    void pre_loop()
    {
       
    }
    void post_loop() {

    }
    void loop()
    {        
        line.Discover().FindNextDevice(dallas);

        float temp = dallas.GetTemperature<float>(line);
        dallas.Update(line);
        uint8_t buffer[5] = {0,};
        memcpy(&buffer[1], (uint8_t *)&temp, 4);
        this->NotifyCharacteristic->writeValue(buffer, sizeof(buffer));
    }
};