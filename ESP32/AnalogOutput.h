#ifndef AnalogOutput_h
#define AnalogOutput_h

#include <Arduino.h>

class AnalogOutput {
    public:
        AnalogOutput(uint8_t pin);
        void on(uint8_t level);
        void off();
    private:
        uint8_t _pin;
};

#endif