#include "Arduino.h"
#include "AnalogInput.h"

AnalogInput::AnalogInput(uint8_t pin, int16_t lowerLimit, int16_t upperLimit) {
    _pin = pin;
    _lowerLimit = lowerLimit;
    _upperLimit = upperLimit;
    pinMode(_pin, INPUT);
}

uint16_t AnalogInput::readRaw() {
    return analogRead(_pin);
}

float AnalogInput::read() {
    return float(readRaw()) // Calculate some value based on lower and upper limits
}
