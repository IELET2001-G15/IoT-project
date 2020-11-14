/**
 * AnalogIO.cpp - Library for reading and writing to ESP32 pins.
 * Created by Espen Holsen, November 14, 2020.
 * Released into the public domain.
*/

#include <Arduino.h>
#include <analogWrite.h>
#include "AnalogIO.h"

AnalogIO::AnalogIO(uint8_t pin, uint8_t mode) {
    pinMode(pin, mode);
    _pin = pin;
}

uint16_t AnalogIO::read() {
    return analogRead(_pin);
}

void AnalogIO::write(uint8_t value) {
    analogWrite(_pin, value);
}
