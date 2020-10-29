#include "Arduino.h"
#include "AnalogOutput.h"
#include "analogWrite.h"

AnalogOutput::AnalogOutput(uint8_t pin) {
    _pin = pin;
    pinMode(_pin, OUTPUT);
}

void AnalogOutput::on(uint8_t level = 100) {
    analogWrite(_pin, map(level, 0, 100, 0, 255));
}

void AnalogOutput::off() {
    on(0);
}