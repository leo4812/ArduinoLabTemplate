#include <vector>
#include "BaseSensor.hpp"
#include "LPS22HB.hpp"
#include "MagnetPole.hpp"
#include "SEN0304.hpp"
#include "MLX90614.hpp"
#include "MAX30102.hpp"
#include "RadSens.hpp"
#include "MPX5700DP.hpp"
#include "MPX5050GP.hpp"
#include "TDS.hpp"
#include "PH.hpp"
#include "voltage.hpp"
#include "ACS712.hpp"
#include "DS18B20.hpp"
//#include "BLUXV30B.hpp"
//#include "MAX31855.hpp"
#include "SHT20.hpp"
#include "M135Q.hpp"
#include "M7Q.hpp"
#include "HW416.hpp"
#include "Dinamometr.hpp"
#include "SoilMoisture.hpp"
#include "Turbidity.hpp"
#include "PNO3.hpp"
#include "Sound.hpp"
#include "BloodPressure.hpp"
#include "Pcl.hpp"
#include "Electrocardiogram.hpp"
#include "datUF.hpp"
#include "Amoniy.hpp"
#include "Calciy.hpp"




LPS22HB *LPS22HB_sensor;
MagnetPole *MagnetPole_sensor;
SEN0304 *SEN0304_sensor;
MLX90614 *MLX90614_sensor;
MAX30102 *MAX30102_sensor;
RadSens *RadSens_sensor;
MPX5700DP *MPX5700DP_sensor;
MPX5050GP *MPX5050GP_sensor;
TDS *TDS_sensor;
PH *PH_sensor;
voltage *voltage_sensor;
ACS712 *ACS712_sensor;
DS18B20 *DS18B20_sensor;
// BLUXV30B * BLUXV30B_sensor;
// MAX31855 *MAX31855_sensor;
SHT20 *SHT20_sensor;
M135Q *M135Q_sensor;
M7Q *M7Q_sensor;
HW416 *HW416_sensor;
Dinamometr *Dinamometr_sensor;
SoilMoisture *SoilMoisture_sensor;
Turbidity *Turbidity_sensor;
PNO3 *PNO3_sensor;
Sound *Sound_sensor;
BloodPressure *BloodPressure_sensor;
Pcl *Pcl_sensor;
Electrocardiogram *Electrocardiogram_sensor;
datUF *datUF_sensor;
Amoniy *Amoniy_sensor;
Calciy *Calciy_sensor;







void init_sensors(BLEService service)
{
    LPS22HB_sensor = new LPS22HB();
    auto LPS22HB_sensor_command_callback = [](BLEDevice device, BLECharacteristic characteristic)
    { LPS22HB_sensor->CommandHandler(device, characteristic); };
    LPS22HB_sensor->CommandCharacteristic->setEventHandler(BLEWritten, LPS22HB_sensor_command_callback);
    service.addCharacteristic(*(LPS22HB_sensor->CommandCharacteristic));
    service.addCharacteristic(*(LPS22HB_sensor->NotifyCharacteristic));

    MagnetPole_sensor = new MagnetPole();
    auto MagnetPole_sensor_command_callback = [](BLEDevice device, BLECharacteristic characteristic)
    { MagnetPole_sensor->CommandHandler(device, characteristic); };
    MagnetPole_sensor->CommandCharacteristic->setEventHandler(BLEWritten, MagnetPole_sensor_command_callback);
    service.addCharacteristic(*(MagnetPole_sensor->CommandCharacteristic));
    service.addCharacteristic(*(MagnetPole_sensor->NotifyCharacteristic));

    SEN0304_sensor = new SEN0304();
    auto SEN0304_sensor_command_callback = [](BLEDevice device, BLECharacteristic characteristic)
    { SEN0304_sensor->CommandHandler(device, characteristic); };
    SEN0304_sensor->CommandCharacteristic->setEventHandler(BLEWritten, SEN0304_sensor_command_callback);
    service.addCharacteristic(*(SEN0304_sensor->CommandCharacteristic));
    service.addCharacteristic(*(SEN0304_sensor->NotifyCharacteristic));

    MLX90614_sensor = new MLX90614();
    auto MLX90614_sensor_command_callback = [](BLEDevice device, BLECharacteristic characteristic)
    { MLX90614_sensor->CommandHandler(device, characteristic); };
    MLX90614_sensor->CommandCharacteristic->setEventHandler(BLEWritten, MLX90614_sensor_command_callback);
    service.addCharacteristic(*(MLX90614_sensor->CommandCharacteristic));
    service.addCharacteristic(*(MLX90614_sensor->NotifyCharacteristic));

    MAX30102_sensor = new MAX30102();
    auto MAX30102_sensor_command_callback = [](BLEDevice device, BLECharacteristic characteristic)
    { MAX30102_sensor->CommandHandler(device, characteristic); };
    MAX30102_sensor->CommandCharacteristic->setEventHandler(BLEWritten, MAX30102_sensor_command_callback);
    service.addCharacteristic(*(MAX30102_sensor->CommandCharacteristic));
    service.addCharacteristic(*(MAX30102_sensor->NotifyCharacteristic));

    RadSens_sensor = new RadSens();
    auto RadSens_sensor_command_callback = [](BLEDevice device, BLECharacteristic characteristic)
    { RadSens_sensor->CommandHandler(device, characteristic); };
    RadSens_sensor->CommandCharacteristic->setEventHandler(BLEWritten, RadSens_sensor_command_callback);
    service.addCharacteristic(*(RadSens_sensor->CommandCharacteristic));
    service.addCharacteristic(*(RadSens_sensor->NotifyCharacteristic));

    MPX5700DP_sensor = new MPX5700DP();
    auto MPX5700DP_sensor_command_callback = [](BLEDevice device, BLECharacteristic characteristic)
    { MPX5700DP_sensor->CommandHandler(device, characteristic); };
    MPX5700DP_sensor->CommandCharacteristic->setEventHandler(BLEWritten, MPX5700DP_sensor_command_callback);
    service.addCharacteristic(*(MPX5700DP_sensor->CommandCharacteristic));
    service.addCharacteristic(*(MPX5700DP_sensor->NotifyCharacteristic));

    MPX5050GP_sensor = new MPX5050GP();
    auto MPX5050GP_sensor_command_callback = [](BLEDevice device, BLECharacteristic characteristic)
    { MPX5050GP_sensor->CommandHandler(device, characteristic); };
    MPX5050GP_sensor->CommandCharacteristic->setEventHandler(BLEWritten, MPX5050GP_sensor_command_callback);
    service.addCharacteristic(*(MPX5050GP_sensor->CommandCharacteristic));
    service.addCharacteristic(*(MPX5050GP_sensor->NotifyCharacteristic));

    TDS_sensor = new TDS();
    auto TDS_sensor_command_callback = [](BLEDevice device, BLECharacteristic characteristic)
    { TDS_sensor->CommandHandler(device, characteristic); };
    TDS_sensor->CommandCharacteristic->setEventHandler(BLEWritten, TDS_sensor_command_callback);
    service.addCharacteristic(*(TDS_sensor->CommandCharacteristic));
    service.addCharacteristic(*(TDS_sensor->NotifyCharacteristic));

    PH_sensor = new PH();
    auto PH_sensor_command_callback = [](BLEDevice device, BLECharacteristic characteristic)
    { PH_sensor->CommandHandler(device, characteristic); };
    PH_sensor->CommandCharacteristic->setEventHandler(BLEWritten, PH_sensor_command_callback);
    service.addCharacteristic(*(PH_sensor->CommandCharacteristic));
    service.addCharacteristic(*(PH_sensor->NotifyCharacteristic));

    voltage_sensor = new voltage();
    auto voltage_sensor_command_callback = [](BLEDevice device, BLECharacteristic characteristic)
    { voltage_sensor->CommandHandler(device, characteristic); };
    voltage_sensor->CommandCharacteristic->setEventHandler(BLEWritten, voltage_sensor_command_callback);
    service.addCharacteristic(*(voltage_sensor->CommandCharacteristic));
    service.addCharacteristic(*(voltage_sensor->NotifyCharacteristic));

    ACS712_sensor = new ACS712();
    auto ACS712_sensor_command_callback = [](BLEDevice device, BLECharacteristic characteristic)
    { ACS712_sensor->CommandHandler(device, characteristic); };
    ACS712_sensor->CommandCharacteristic->setEventHandler(BLEWritten, ACS712_sensor_command_callback);
    service.addCharacteristic(*(ACS712_sensor->CommandCharacteristic));
    service.addCharacteristic(*(ACS712_sensor->NotifyCharacteristic));

    DS18B20_sensor = new DS18B20();
    auto DS18B20_sensor_command_callback = [](BLEDevice device, BLECharacteristic characteristic)
    { DS18B20_sensor->CommandHandler(device, characteristic); };
    DS18B20_sensor->CommandCharacteristic->setEventHandler(BLEWritten, DS18B20_sensor_command_callback);
    service.addCharacteristic(*(DS18B20_sensor->CommandCharacteristic));
    service.addCharacteristic(*(DS18B20_sensor->NotifyCharacteristic));
    /*

        BLUXV30B_sensor = new BLUXV30B();
        auto BLUXV30B_sensor_command_callback = [](BLEDevice device, BLECharacteristic characteristic) { BLUXV30B_sensor->CommandHandler(device, characteristic); };
        BLUXV30B_sensor->CommandCharacteristic->setEventHandler(BLEWritten, BLUXV30B_sensor_command_callback);
        service.addCharacteristic(*(BLUXV30B_sensor->CommandCharacteristic));
        service.addCharacteristic(*(BLUXV30B_sensor->NotifyCharacteristic));



        MAX31855_sensor = new MAX31855();
        auto MAX31855_sensor_command_callback = [](BLEDevice device, BLECharacteristic characteristic) { MAX31855_sensor->CommandHandler(device, characteristic); };
        MAX31855_sensor->CommandCharacteristic->setEventHandler(BLEWritten, MAX31855_sensor_command_callback);
        service.addCharacteristic(*(MAX31855_sensor->CommandCharacteristic));
        service.addCharacteristic(*(MAX31855_sensor->NotifyCharacteristic));
        */

    SHT20_sensor = new SHT20();
    auto SHT20_sensor_command_callback = [](BLEDevice device, BLECharacteristic characteristic)
    { SHT20_sensor->CommandHandler(device, characteristic); };
    SHT20_sensor->CommandCharacteristic->setEventHandler(BLEWritten, SHT20_sensor_command_callback);
    service.addCharacteristic(*(SHT20_sensor->CommandCharacteristic));
    service.addCharacteristic(*(SHT20_sensor->NotifyCharacteristic));

    M135Q_sensor = new M135Q();
    auto M135Q_sensor_command_callback = [](BLEDevice device, BLECharacteristic characteristic)
    { M135Q_sensor->CommandHandler(device, characteristic); };
    M135Q_sensor->CommandCharacteristic->setEventHandler(BLEWritten, M135Q_sensor_command_callback);
    service.addCharacteristic(*(M135Q_sensor->CommandCharacteristic));
    service.addCharacteristic(*(M135Q_sensor->NotifyCharacteristic));

    M7Q_sensor = new M7Q();
    auto M7Q_sensor_command_callback = [](BLEDevice device, BLECharacteristic characteristic)
    { M7Q_sensor->CommandHandler(device, characteristic); };
    M7Q_sensor->CommandCharacteristic->setEventHandler(BLEWritten, M7Q_sensor_command_callback);
    service.addCharacteristic(*(M7Q_sensor->CommandCharacteristic));
    service.addCharacteristic(*(M7Q_sensor->NotifyCharacteristic));

    HW416_sensor = new HW416();
    auto HW416_sensor_command_callback = [](BLEDevice device, BLECharacteristic characteristic)
    { HW416_sensor->CommandHandler(device, characteristic); };
    HW416_sensor->CommandCharacteristic->setEventHandler(BLEWritten, HW416_sensor_command_callback);
    service.addCharacteristic(*(HW416_sensor->CommandCharacteristic));
    service.addCharacteristic(*(HW416_sensor->NotifyCharacteristic));

    Dinamometr_sensor = new Dinamometr();
    auto Dinamometr_sensor_command_callback = [](BLEDevice device, BLECharacteristic characteristic)
    { Dinamometr_sensor->CommandHandler(device, characteristic); };
    Dinamometr_sensor->CommandCharacteristic->setEventHandler(BLEWritten, Dinamometr_sensor_command_callback);
    service.addCharacteristic(*(Dinamometr_sensor->CommandCharacteristic));
    service.addCharacteristic(*(Dinamometr_sensor->NotifyCharacteristic));

    SoilMoisture_sensor = new SoilMoisture();
    auto SoilMoisture_sensor_command_callback = [](BLEDevice device, BLECharacteristic characteristic)
    { SoilMoisture_sensor->CommandHandler(device, characteristic); };
    SoilMoisture_sensor->CommandCharacteristic->setEventHandler(BLEWritten, SoilMoisture_sensor_command_callback);
    service.addCharacteristic(*(SoilMoisture_sensor->CommandCharacteristic));
    service.addCharacteristic(*(SoilMoisture_sensor->NotifyCharacteristic));

    Turbidity_sensor = new Turbidity();
    auto Turbidity_sensor_command_callback = [](BLEDevice device, BLECharacteristic characteristic)
    { Turbidity_sensor->CommandHandler(device, characteristic); };
    Turbidity_sensor->CommandCharacteristic->setEventHandler(BLEWritten, Turbidity_sensor_command_callback);
    service.addCharacteristic(*(Turbidity_sensor->CommandCharacteristic));
    service.addCharacteristic(*(Turbidity_sensor->NotifyCharacteristic));

    PNO3_sensor = new PNO3();
    auto PNO3_sensor_command_callback = [](BLEDevice device, BLECharacteristic characteristic)
    { PNO3_sensor->CommandHandler(device, characteristic); };
    PNO3_sensor->CommandCharacteristic->setEventHandler(BLEWritten, PNO3_sensor_command_callback);
    service.addCharacteristic(*(PNO3_sensor->CommandCharacteristic));
    service.addCharacteristic(*(PNO3_sensor->NotifyCharacteristic));

    Sound_sensor = new Sound();
    auto Sound_sensor_command_callback = [](BLEDevice device, BLECharacteristic characteristic)
    { Sound_sensor->CommandHandler(device, characteristic); };
    Sound_sensor->CommandCharacteristic->setEventHandler(BLEWritten, Sound_sensor_command_callback);
    service.addCharacteristic(*(Sound_sensor->CommandCharacteristic));
    service.addCharacteristic(*(Sound_sensor->NotifyCharacteristic));

    BloodPressure_sensor = new BloodPressure();
    auto BloodPressure_sensor_command_callback = [](BLEDevice device, BLECharacteristic characteristic)
    { BloodPressure_sensor->CommandHandler(device, characteristic); };
    BloodPressure_sensor->CommandCharacteristic->setEventHandler(BLEWritten, BloodPressure_sensor_command_callback);
    service.addCharacteristic(*(BloodPressure_sensor->CommandCharacteristic));
    service.addCharacteristic(*(BloodPressure_sensor->NotifyCharacteristic));

    Pcl_sensor = new Pcl();
    auto Pcl_sensor_command_callback = [](BLEDevice device, BLECharacteristic characteristic)
    { Pcl_sensor->CommandHandler(device, characteristic); };
    Pcl_sensor->CommandCharacteristic->setEventHandler(BLEWritten, Pcl_sensor_command_callback);
    service.addCharacteristic(*(Pcl_sensor->CommandCharacteristic));
    service.addCharacteristic(*(Pcl_sensor->NotifyCharacteristic));

    Electrocardiogram_sensor = new Electrocardiogram();
    auto Electrocardiogram_sensor_command_callback = [](BLEDevice device, BLECharacteristic characteristic)
    { Electrocardiogram_sensor->CommandHandler(device, characteristic); };
    Electrocardiogram_sensor->CommandCharacteristic->setEventHandler(BLEWritten, Electrocardiogram_sensor_command_callback);
    service.addCharacteristic(*(Electrocardiogram_sensor->CommandCharacteristic));
    service.addCharacteristic(*(Electrocardiogram_sensor->NotifyCharacteristic));

    datUF_sensor = new datUF();
    auto datUF_sensor_command_callback = [](BLEDevice device, BLECharacteristic characteristic)
    { datUF_sensor->CommandHandler(device, characteristic); };
    datUF_sensor->CommandCharacteristic->setEventHandler(BLEWritten, datUF_sensor_command_callback);
    service.addCharacteristic(*(datUF_sensor->CommandCharacteristic));
    service.addCharacteristic(*(datUF_sensor->NotifyCharacteristic));

    Amoniy_sensor = new Amoniy();
    auto Amoniy_sensor_command_callback = [](BLEDevice device, BLECharacteristic characteristic)
    { Amoniy_sensor->CommandHandler(device, characteristic); };
    Amoniy_sensor->CommandCharacteristic->setEventHandler(BLEWritten, Amoniy_sensor_command_callback);
    service.addCharacteristic(*(Amoniy_sensor->CommandCharacteristic));
    service.addCharacteristic(*(Amoniy_sensor->NotifyCharacteristic));

    Calciy_sensor = new Calciy();
    auto Calciy_sensor_command_callback = [](BLEDevice device, BLECharacteristic characteristic)
    { Calciy_sensor->CommandHandler(device, characteristic); };
    Calciy_sensor->CommandCharacteristic->setEventHandler(BLEWritten, Calciy_sensor_command_callback);
    service.addCharacteristic(*(Calciy_sensor->CommandCharacteristic));
    service.addCharacteristic(*(Calciy_sensor->NotifyCharacteristic));
}