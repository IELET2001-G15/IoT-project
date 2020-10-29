#ifndef AnalogInput_h
#define AnalogInput_h

#include "Arduino.h"

class AnalogInput {
    public:
        AnalogInput(uint8_t pin);
        uint16_t readRaw();
        float read();
    private:
        uint8_t _pin;
        int16_t _lowerLimit;
        int16_t _upperLimit;
};

#endif