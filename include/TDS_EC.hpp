#include <Arduino.h>
#include "BaseSensor.hpp"
#include <Wire.h>             // * Подключаем библиотеку для работы с аппаратной шиной I2C.
#include <iarduino_I2C_TDS.h> //   Подключаем библиотеку для работы с TDS/EC-метром I2C-flash.

class TDS_EC : public BaseSensor
{

public:
    iarduino_I2C_TDS *tds; //   Объявляем объект tds  для работы с функциями и методами библиотеки iarduino_I2C_TDS, указывая адрес модуля на шине I2C.

    TDS_EC()
    {
        this->Name = (char *)std::string("TDS_EC").c_str();
        CommandCharacteristic = new BLECharacteristic("3bbf2906-e385-4612-8e8f-7e2ad2e30c4c", BLERead | BLEWrite, DIGITAL_COMMAND_SIZE, true);
        NotifyCharacteristic = new BLECharacteristic("7a6cd389-e947-49ba-b135-f17735ed5cb8", BLERead | BLENotify, 9, true);
        tds = new iarduino_I2C_TDS(0x09);
    }

private:
    void pre_loop()
    {
        tds->begin(); //   Инициируем работу с TDS/EC-метром I2C-flash.
    }
    void post_loop() {}
    void loop()
    {

        tds->set_t(25.0f); //   Указываем текущую температуру жидкости.

        uint32_t EC = tds->getEC();   //Микросименс на см
        uint32_t TDS = tds->getTDS(); // PPM

        uint8_t buffer[9] = {
            0,
        };
        buffer[0] = 0;
        memcpy(&buffer[1], (uint8_t *)&EC, 4);
        memcpy(&buffer[5], (uint8_t *)&TDS, 4);
        if (this->isSerialConnection)
        {
            String strHEX = buffToHex(&buffer[0], 9);

            String Val = "7a6cd389-e947-49ba-b135-f17735ed5cb8";
            Val += ";";
            Val += strHEX;
            Serial.println(Val);
        }
        else
        {
            this->NotifyCharacteristic->writeValue(buffer, sizeof(buffer));
        }
    }
};