#ifndef _TEMPL_SENSORS_H_
#define _TEMPL_SENSORS_H_
#include <ArduinoBLE.h>
#include <rtos.h>
//#include <mbed.h>
#include <sstream>
#include <iomanip>
using namespace mbed;
using namespace rtos;

#define ANALOG_COMMAND_SIZE 6
#define DIGITAL_COMMAND_SIZE 5
class BaseSensor
{
public:
    BaseSensor()
    {
    }
    BLECharacteristic *CommandCharacteristic;
    BLECharacteristic *NotifyCharacteristic;
    bool IsAnalog = false;
    uint32_t PoolingInterval = 0;
    uint8_t AnalogPort = 0;
    char *Name = nullptr;
    void CommandHandler(BLEDevice device, BLECharacteristic characteristic)
    {
        uint8_t buffer[8];
        characteristic.readValue(buffer, 8);
        uint8_t command = buffer[0];
        if (command == 0x01)
        {
            this->PoolingInterval = (uint32_t)(buffer[1] << 24) | (uint32_t)(buffer[2] << 16) | (uint32_t)(buffer[3] << 8) | (uint32_t)buffer[4];
            Serial.println(this->PoolingInterval);
            printf("Starting %s with %lu ms interval\n", this->Name, this->PoolingInterval);
            if (this->IsAnalog)
            {
                this->AnalogPort = buffer[5] == 0x00 ? A0 : A1;
                printf("Analog port is %d (%d)\n", buffer[5], this->AnalogPort);
            }

            this->run_thread = new Thread();
            this->run_thread->start(callback(this, &BaseSensor::execute));
        }
        else
        {
            printf("Stopping %s\n", this->Name);
            if (this->run_thread)
            {
                this->run_thread->terminate();
                delete this->run_thread;
                this->post_loop();
            }
        }
    }

private:
    Thread *run_thread = nullptr;
    void execute()
    {
        this->pre_loop();
        for (;;)
        {
            uint32_t start_time = micros() / 1000;

            this->loop();

            uint32_t end_time = micros() / 1000;
            int32_t delta = PoolingInterval - (end_time - start_time);
            if (delta > 0)
                rtos::ThisThread::sleep_for(std::chrono::milliseconds(delta));
            rtos::ThisThread::yield();
        }
    }
    virtual void loop() = 0;
    virtual void pre_loop() = 0;
    virtual void post_loop() = 0;
};

#endif