#pragma once
#include <Arduino.h>
#include <mbed.h>
class PulseCounter
{
public:
    PulseCounter()
    {
        this->clb = mbed::callback(this, &PulseCounter::interruptCallback);
    }
    volatile uint32_t counter = 0;

    uint32_t pulsePeriod(uint32_t elapsedTime)
    {
        uint32_t result = 0;
        if (counter > 0)
        {
            result = elapsedTime / counter;
            counter = 0;
        }
        return result;
    }

    void disableInterrupt()
    {
        detachInterrupt(_interruptNum);
    }

    void enableInterrupt(pin_size_t interruptNum, PinStatus mode, bool useAutoPull = true)
    {
        if (interruptNum >= PINS_COUNT)
        {
            printf("No interrupt for pin %d\n", interruptNum);
            return;
        }
        this->_interruptNum = interruptNum;

        detachInterrupt(interruptNum);
        mbed::InterruptIn *irq = new mbed::InterruptIn(digitalPinToPinName(interruptNum), PullNone);
        if (mode == CHANGE)
        {
            irq->rise(this->clb);
            irq->fall(this->clb);
        }
        else if (mode == FALLING)
        {
            irq->fall(this->clb);
        }
        else
        {
            irq->rise(this->clb);
        }
         if (useAutoPull)
         {
             if (mode == FALLING)
             {
                 pinMode(interruptNum, INPUT_PULLUP);
             }
             else if (mode == RISING)
             {
                 pinMode(interruptNum, INPUT_PULLDOWN);
             }
             else
             {
                 pinMode(interruptNum, INPUT);
             }
         }
        printf("Interrupt enabled on pin %d\n", interruptNum);
    }

private:
    pin_size_t _interruptNum;
    mbed::Callback<void()> clb;
    void interruptCallback()
    {        
        counter++;
    }
};