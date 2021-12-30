#include <Arduino.h>
#include "BaseSensor.hpp"

class M7Q : public BaseSensor
{

public:
    M7Q()
    {
        this->Name = (char *)std::string("M7Q").c_str();
        this->IsAnalog = true;
        CommandCharacteristic = new BLECharacteristic("C0E294C1-029A-4421-AA13-DE0C7E16A21B", BLERead | BLEWrite, ANALOG_COMMAND_SIZE, true);
        NotifyCharacteristic = new BLECharacteristic("A93C4467-02F7-45AE-87E6-3BA799560BD5", BLERead | BLENotify, 8, true);
    }

private:
// MQ7PIN (this->AnalogPort)       // define which analog input channel you are going to use
#define RL_VALUE_MQ7 (1)                // define the load resistance on the board, in kilo ohms
#define RO_CLEAN_AIR_FACTOR_MQ7 (26.09) // RO_CLEAR_AIR_FACTOR=(Sensor resistance in clean air)/RO,
                                        // which is derived from the chart in datasheet

/***********************Software Related Macros************************************/
#define CALIBARAION_SAMPLE_TIMES (50)     // define how many samples you are going to take in the calibration phase
#define CALIBRATION_SAMPLE_INTERVAL (500) // define the time interal(in milisecond) between each samples in the
                                          // cablibration phase
#define READ_SAMPLE_INTERVAL (50)         // define how many samples you are going to take in normal operation
#define READ_SAMPLE_TIMES (5)             // define the time interal(in milisecond) between each samples in
                                          // normal operation

/**********************Application Related Macros**********************************/
#define GAS_CARBON_MONOXIDE (3)
#define GAS_HYDROGEN (0)
#define GAS_LPG (1)
#define GAS_METHANE (2)
#define GAS_ALCOHOL (4)
#define accuracy (0) // for linearcurves
    //#define         accuracy                   (1)   //for nonlinearcurves, un comment this line and comment the above line if calculations
    // are to be done using non linear curve equations
    /*****************************Globals***********************************************/
    float Ro = 10; // Ro is initialized to 10 kilo ohms

    void pre_loop()
    {

        Ro = MQCalibration(this->AnalogPort);
    }
    void post_loop() {}
    void loop()
    {
        float co = MQGetGasPercentage(MQRead(this->AnalogPort)/Ro,GAS_CARBON_MONOXIDE);
        uint16_t value = analogRead(this->AnalogPort);

        Serial.print("Пощитаное значение: ");
        Serial.print(co);
        Serial.print("     Попугаи: ");
        Serial.println(value);

        uint8_t buffer[8] = {
            0,
        };

        buffer[1] = this->AnalogPort == A0 ? 0x00 : 0x01;

        memcpy(&buffer[2], (uint8_t *)&co, 4);

        memcpy(&buffer[6], &value, 2);
        this->NotifyCharacteristic->writeValue(buffer, sizeof(buffer));
    }

    float MQResistanceCalculation(int raw_adc)
    {
        return (((float)RL_VALUE_MQ7 * (1023 - raw_adc) / raw_adc));
    }

    /***************************** MQCalibration ****************************************
    Input:   mq_pin - analog channel
    Output:  Ro of the sensor
    Remarks: This function assumes that the sensor is in clean air. It use
             MQResistanceCalculation to calculates the sensor resistance in clean air
             and then divides it with RO_CLEAN_AIR_FACTOR. RO_CLEAN_AIR_FACTOR is about
             10, which differs slightly between different sensors.
    ************************************************************************************/
    float MQCalibration(int mq_pin)
    {
        int i;
        float RS_AIR_val = 0, r0;

        for (i = 0; i < CALIBARAION_SAMPLE_TIMES; i++)
        { // take multiple samples
            RS_AIR_val += MQResistanceCalculation(analogRead(mq_pin));
            delay(CALIBRATION_SAMPLE_INTERVAL);
        }
        RS_AIR_val = RS_AIR_val / CALIBARAION_SAMPLE_TIMES; // calculate the average value

        r0 = RS_AIR_val / RO_CLEAN_AIR_FACTOR_MQ7; // RS_AIR_val divided by RO_CLEAN_AIR_FACTOR yields the Ro
                                                   // according to the chart in the datasheet

        return r0;
    }

    /*****************************  MQRead *********************************************
    Input:   mq_pin - analog channel
    Output:  Rs of the sensor
    Remarks: This function use MQResistanceCalculation to caculate the sensor resistenc (Rs).
             The Rs changes as the sensor is in the different consentration of the target
             gas. The sample times and the time interval between samples could be configured
             by changing the definition of the macros.
    ************************************************************************************/
    float MQRead(int mq_pin)
    {
        int i;
        float rs = 0;

        for (i = 0; i < READ_SAMPLE_TIMES; i++)
        {
            rs += MQResistanceCalculation(analogRead(mq_pin));
            delay(READ_SAMPLE_INTERVAL);
        }

        rs = rs / READ_SAMPLE_TIMES;

        return rs;
    }

    /*****************************  MQGetGasPercentage **********************************
    Input:   rs_ro_ratio - Rs divided by Ro
             gas_id      - target gas type
    Output:  ppm of the target gas
    Remarks: This function uses different equations representing curves of each gas to
             calculate the ppm (parts per million) of the target gas.
    ************************************************************************************/
    int MQGetGasPercentage(float rs_ro_ratio, int gas_id)
    {
        if (accuracy == 0)
        {
            if (gas_id == GAS_CARBON_MONOXIDE)
            {
                return (pow(10, ((-1.525 * (log10(rs_ro_ratio))) + 1.994)));
            }
            else if (gas_id == GAS_HYDROGEN)
            {
                return (pow(10, ((-1.355 * (log10(rs_ro_ratio))) + 1.847)));
            }
            else if (gas_id == GAS_LPG)
            {
                return (pow(10, ((-7.622 * (log10(rs_ro_ratio))) + 8.919)));
            }
            else if (gas_id == GAS_METHANE)
            {
                return (pow(10, ((-11.01 * (log10(rs_ro_ratio))) + 14.32)));
            }
            else if (gas_id == GAS_ALCOHOL)
            {
                return (pow(10, ((-14.72 * (log10(rs_ro_ratio))) + 19.31)));
            }
        }

        else if (accuracy == 1)
        {
            if (gas_id == GAS_CARBON_MONOXIDE)
            {
                return (pow(10, ((-1.525 * (log10(rs_ro_ratio))) + 1.994)));
            }
            else if (gas_id == GAS_HYDROGEN)
            {
                return (pow(10, ((-1.355 * (log10(rs_ro_ratio))) + 1.847)));
            }
            else if (gas_id == GAS_LPG)
            {
                return (pow(10, (61.00 * pow((log10(rs_ro_ratio)), 3) - 148.7 * pow((log10(rs_ro_ratio)), 2) + 112.6 * (log10(rs_ro_ratio)) - 23.30)));
            }
            else if (gas_id == GAS_METHANE)
            {
                return (pow(10, (3480 * pow((log10(rs_ro_ratio)), 4) - 14193 * pow((log10(rs_ro_ratio)), 3) + 21627 * pow((log10(rs_ro_ratio)), 2) - 14606 * (log10(rs_ro_ratio)) + 3694)));
            }
            else if (gas_id == GAS_ALCOHOL)
            {
                return (pow(10, (-25729 * pow((log10(rs_ro_ratio)), 4) + 11746 * pow((log10(rs_ro_ratio)), 3) - 20087 * pow((log10(rs_ro_ratio)), 2) + 15250 * (log10(rs_ro_ratio)) - 43357)));
            }
        }
        return 0;
    }
};