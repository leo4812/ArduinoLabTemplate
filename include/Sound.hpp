#include <Arduino.h>
#include "BaseSensor.hpp"
#include "VolAnalyzer.h"

using namespace rtos;

class Sound : public BaseSensor
{

public:
    VolAnalyzer *analyzer = nullptr;

    volatile int inSound = 0;

    bool doWork = true;

    // this->analyzer = new VolAnalyzer(this->AnalogPort); // Тут не работает

    volatile uint32_t outSound = 0; // пометить как volatile, глобальная переменная для отдельного потока

    Thread *runThread;

    Sound()
    {

        this->Name = (char *)std::string("Sound").c_str();
        this->IsAnalog = true;
        CommandCharacteristic = new BLECharacteristic("5A1FBAEC-8E0D-4DCC-9BC6-5FF89A5FA849", BLERead | BLEWrite, ANALOG_COMMAND_SIZE, true);
        NotifyCharacteristic = new BLECharacteristic("DC317121-AEE3-486C-A14F-90C52EDBCB8B", BLERead | BLENotify, 6, true);

        this->analyzer = new VolAnalyzer(this->AnalogPort);
    }

private:
    // this->analyzer = new VolAnalyzer(this->AnalogPort); // Тут не работает

    void execute()
    {
        while (doWork)
        {
            if (this->analyzer->tick())
            {
                inSound = this->analyzer->getMax();
<<<<<<< HEAD
                outSound = map(inSound, 0, 650, 20, 110);
                Serial.println(inSound);
=======
                outSound = map(inSound, 100, 2100, 20, 110);
                /*
                Serial.print("IN: ");
                Serial.print(inSound);
                Serial.print("   OUT: ");
                Serial.println(outSound);
                */
>>>>>>> 27be58b2e6702890b15a78d61feda2aff27cc38c
            }
        }
    }
    void pre_loop()
    {
<<<<<<< HEAD
        // this->analyzer = new VolAnalyzer(this->AnalogPort); // Компилит но не работает (показания всегда 20-21)
=======
        this->analyzer = new VolAnalyzer(this->AnalogPort);

>>>>>>> 27be58b2e6702890b15a78d61feda2aff27cc38c
        this->analyzer->setVolK(20);
        this->analyzer->setTrsh(10);
        this->analyzer->setVolMin(10);
        this->analyzer->setVolMax(100);

        doWork = true;
        runThread = new Thread();
        runThread->start(callback(this, &Sound::execute));
    }
    void post_loop()
    {
        doWork = false;
        if (runThread != nullptr)
        {
            runThread = nullptr;
        }
    }
    void loop()
    {
        uint32_t value = outSound;
        uint8_t buffer[6] = {
            0,
        };
        buffer[1] = this->AnalogPort == A0 ? 0x00 : 0x01;
        memcpy(&buffer[2], &value, sizeof(value));
        this->NotifyCharacteristic->writeValue(buffer, sizeof(buffer));
    }
};
