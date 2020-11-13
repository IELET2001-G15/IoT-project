/*
  Morse.h - Library for flashing Morse code.
  Created by David A. Mellis, November 2, 2007.
  Released into the public domain.
*/
#ifndef AnalogIO_h
#define AnalogIO_h

#include <Arduino.h>

/**
 * Contains all methods and attibutes related to analog input and output. Includes the methods
 * read() for reading the value on a pin, and write() to write a specific value to a pin
 * @param pin the pin number the I/O is connected to
 * @param mode the mode of the I/O. INPUT, OUTPUT, INPUT_PULLUP, etc...
*/
class AnalogIO {
    public:
        AnalogIO(uint8_t pin, uint8_t mode);
        uint16_t read();
        void write(uint8_t value);
    private:
        uint8_t _pin;
};

#endif