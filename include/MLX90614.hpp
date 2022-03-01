#include <Arduino.h>
#include "BaseSensor.hpp"
#include <Wire.h>
#include <Adafruit_MLX90614.h>

Adafruit_MLX90614 mlx = Adafruit_MLX90614();

class MLX90614 : public BaseSensor
{

public:
    bool ErrorMLX90614 = false; //Датчик не подключен

    MLX90614()
    {
        this->Name = (char *)std::string("MLX90614").c_str();
        CommandCharacteristic = new BLECharacteristic("5cc8c9ca-eebe-4561-8377-7223f2bc1086", BLERead | BLEWrite, DIGITAL_COMMAND_SIZE, true);
        NotifyCharacteristic = new BLECharacteristic("343b032b-89b8-4b8e-8726-90b156cf2338", BLERead | BLENotify, 5, true);
    }

private:
    void pre_loop()
    {
        ErrorMLX90614 = !mlx.begin();
    }
    void post_loop() {}
    void loop()
    {
        float temp = mlx.readObjectTempC();

        uint8_t buffer[5] = {
            0,
        };
        if (ErrorMLX90614 == true)
        {
            buffer[0] = 1;
        }
        else
        {
            buffer[0] = 0;
            memcpy(&buffer[1], (uint8_t *)&temp, sizeof(temp));
        }
        this->NotifyCharacteristic->writeValue(buffer, sizeof(buffer));
        Serial.println(mlx.begin());  //                                     ТУТ
    }
};