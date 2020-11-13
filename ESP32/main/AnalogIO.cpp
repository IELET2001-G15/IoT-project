/*
  Morse.cpp - Library for flashing Morse code.
  Created by David A. Mellis, November 2, 2007.
  Released into the public domain.
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