#include <Arduino.h>
#include "BaseSensor.hpp"

class Sound : public BaseSensor
{

public:
    Sound()
    {
        this->Name = (char *)std::string("Sound").c_str();
        this->IsAnalog = true;
        CommandCharacteristic = new BLECharacteristic("5A1FBAEC-8E0D-4DCC-9BC6-5FF89A5FA849", BLERead | BLEWrite, ANALOG_COMMAND_SIZE, true);
        NotifyCharacteristic = new BLECharacteristic("DC317121-AEE3-486C-A14F-90C52EDBCB8B", BLERead | BLENotify, 6, true);
    }

private:
    void pre_loop()
    {
        pinMode(this->AnalogPort, INPUT);
    }
    void post_loop() {}
    void loop()
    {
        uint32_t value = analogRead(this->AnalogPort);
        uint8_t buffer[6] = { 0, };
        buffer[1] = this->AnalogPort == A0 ? 0x00 : 0x01;
        memcpy(&buffer[2], &value, sizeof(value));
        this->NotifyCharacteristic->writeValue(buffer, sizeof(buffer));
    }
};