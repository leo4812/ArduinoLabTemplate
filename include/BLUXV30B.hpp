/* #include <Arduino.h>
#include "BaseSensor.hpp"
#include <Adafruit_I2CDevice.h>
#include <Adafruit_BusIO_Register.h>

// НЕ ДОБАВЛЯЛ САМИ БИБЛИОТЕКИ

uint8_t blux_i2c_addr = (uint8_t) 0x4A;

Adafruit_I2CDevice blux_i2c_obj = Adafruit_I2CDevice(blux_i2c_addr);


class BLUXV30B : public BaseSensor
{

public:
    BLUXV30B()
    {
        this->Name = (char *)"BLUXV30B";
        CommandCharacteristic = new BLECharacteristic("B97A0D31-5278-408E-9CBB-D236E8A1A5C3", BLERead | BLEWrite, DIGITAL_COMMAND_SIZE, true);
        NotifyCharacteristic = new BLECharacteristic("BF7C133C-7438-4AEF-A577-3D1BF65B2D1D", BLERead | BLENotify, 5, true);
    }

private:
    void pre_loop()
    {
        blux_i2c_obj.begin();
        
    }
    void post_loop() {}
    void loop()
    {
        Adafruit_BusIO_Register data_reg = Adafruit_BusIO_Register(&blux_i2c_obj, 0x00, 4, LSBFIRST);
        uint32_t val = data_reg.read();
        uint8_t s[5] = {
            0,
        };
        float lux = ((float)val * 1.4) / 1000.0;
        memcpy(&s[1], &lux, sizeof(lux));
    }
};
*/