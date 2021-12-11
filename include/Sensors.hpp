#include <vector>
#include "BaseSensor.hpp"
#include "LPS22HB.hpp"
#include "MagnetPole.hpp"
#include "SEN0304.hpp"


LPS22HB *LPS22HB_sensor;
MagnetPole *MagnetPole_sensor;
SEN0304 *SEN0304_sensor;

void init_sensors(BLEService service)
{
    LPS22HB_sensor = new LPS22HB();
    auto LPS22HB_sensor_command_callback = [](BLEDevice device, BLECharacteristic characteristic) { LPS22HB_sensor->CommandHandler(device, characteristic); };
    LPS22HB_sensor->CommandCharacteristic->setEventHandler(BLEWritten, LPS22HB_sensor_command_callback);
    service.addCharacteristic(*(LPS22HB_sensor->CommandCharacteristic));
    service.addCharacteristic(*(LPS22HB_sensor->NotifyCharacteristic));

    MagnetPole_sensor = new MagnetPole();
    auto MagnetPole_sensor_command_callback = [](BLEDevice device, BLECharacteristic characteristic) { MagnetPole_sensor->CommandHandler(device, characteristic); };
    MagnetPole_sensor->CommandCharacteristic->setEventHandler(BLEWritten, MagnetPole_sensor_command_callback);
    service.addCharacteristic(*(MagnetPole_sensor->CommandCharacteristic));
    service.addCharacteristic(*(MagnetPole_sensor->NotifyCharacteristic));

    SEN0304_sensor = new SEN0304();
    auto SEN0304_sensor_command_callback = [](BLEDevice device, BLECharacteristic characteristic) { SEN0304_sensor->CommandHandler(device, characteristic); };
    SEN0304_sensor->CommandCharacteristic->setEventHandler(BLEWritten, SEN0304_sensor_command_callback);
    service.addCharacteristic(*(SEN0304_sensor->CommandCharacteristic));
    service.addCharacteristic(*(SEN0304_sensor->NotifyCharacteristic));
}