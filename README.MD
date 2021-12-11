# Arduino nano 33 ble sense LAB template project for Platformio

Для добавления нового сенсора, нужно 
1. добавить в папку include файл описывающий сенсор.
Например:  
## Аналоговый датчик
**MagnetPole.hpp**
```cpp
#include <Arduino.h>
#include "BaseSensor.hpp"

class MagnetPole : public BaseSensor
{

public:
    MagnetPole()
    {
        this->Name = "MagnetPole"; // необязательно. Можно использовать в отладке
        this->IsAnalog = true;
        CommandCharacteristic = new BLECharacteristic("3B75281E-00A0-4424-84C5-4C549CC1AE82", BLERead | BLEWrite, ANALOG_COMMAND_SIZE, true);
        NotifyCharacteristic = new BLECharacteristic("EF8A1B0B-1005-4DAD-B49D-75F84488E52C", BLERead | BLENotify, 6, true);
    }

private:
    void pre_loop()
    {
        pinMode(this->AnalogPort, INPUT);
    }
    void post_loop() {}
    void loop()
    {
        uint16_t value = analogRead(this->AnalogPort);
        uint8_t buffer[6] = { 0, };
        buffer[1] = this->AnalogPort == A0 ? 0x00 : 0x01;
        memcpy(&buffer[2], &value, sizeof(value));
        this->NotifyCharacteristic->writeValue(buffer, sizeof(buffer));
    }
};
```
## Цифровой датчик
**LPS22HB.hpp**
```cpp
#include <Arduino.h>
#include "BaseSensor.hpp"
#include <Arduino_LPS22HB.h>

class LPS22HB : public BaseSensor
{

public:
    LPS22HB()
    {
        this->Name = "LPS22HB";
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
```
### Поля
* **CommandCharacteristic** - характеристика приема команд
* **NotifyCharacteristic** - характеристика отправки данных
* **IsAnalog** - признак аналогового датчика
### Методы
* **pre_loop()** - вызывается до начала опроса датчика (loop) при старте потока. Инициализация
* **loop()** - вызывается основным циклом потока. Тут опрос датчика
* **post_loop()** - вызывается при завершении потока опроса, после loop
### Директивы
* **ANALOG_COMMAND_SIZE** - 6 байт
* **DIGITAL_COMMAND_SIZE** - 5 байт

2. Изменить файл **Sensors.hpp**
```cpp
#include <vector>
#include "BaseSensor.hpp"
#include "LPS22HB.hpp"
#include "MagnetPole.hpp"

LPS22HB *LPS22HB_sensor; // указатель на объект датчика
MagnetPole *MagnetPole_sensor;

void init_sensors(BLEService service)
{
    LPS22HB_sensor = new LPS22HB();
    // лямбда фукнция для вызова обработки команды
    auto LPS22HB_sensor_command_callback = [](BLEDevice device, BLECharacteristic characteristic) { LPS22HB_sensor->CommandHandler(device, characteristic); };
    // установка калбака
    LPS22HB_sensor->CommandCharacteristic->setEventHandler(BLEWritten, LPS22HB_sensor_command_callback);
    // добавление характеристик датчика в основной сервис
    service.addCharacteristic(*(LPS22HB_sensor->CommandCharacteristic));
    service.addCharacteristic(*(LPS22HB_sensor->NotifyCharacteristic));

    MagnetPole_sensor = new MagnetPole();
    auto MagnetPole_sensor_command_callback = [](BLEDevice device, BLECharacteristic characteristic) { MagnetPole_sensor->CommandHandler(device, characteristic); };
    MagnetPole_sensor->CommandCharacteristic->setEventHandler(BLEWritten, MagnetPole_sensor_command_callback);
    service.addCharacteristic(*(MagnetPole_sensor->CommandCharacteristic));
    service.addCharacteristic(*(MagnetPole_sensor->NotifyCharacteristic));
}
```




// git push --set-upstream origin Kulikov   (На локальный гит)
// git push -u gitHub   (На гитхаб)