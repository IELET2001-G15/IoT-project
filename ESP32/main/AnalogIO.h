/**
 * AnalogIO.h - Library for reading and writing to ESP32 pins.
 * Created by Espen Holsen, November 14, 2020.
 * Released into the public domain.
*/

#ifndef AnalogIO_h
#define AnalogIO_h

#include <Arduino.h>

/**
 * Contains all methods and attibutes related to analog input and output. Includes the methods
 * read() for reading the value on a pin, and write() to write a specific value to a pin
*/
class AnalogIO {
    public:
        /**
         * @param pin the pin number on the pinout-diagram that the I/O is connected to
         * @param mode the mode of the I/O. For example INPUT, OUTPUT, INPUT_PULLUP, etc...
        */
        AnalogIO(uint8_t pin, uint8_t mode);

        /**
         * Reads the value of the pin
         * @return the 12-bit value in the interval 0-4095
        */
        uint16_t read();

        /**
         * Writes the value to the pin
         * @param value the value in the interval 0-255
        */
        void write(uint8_t value);
    private:
        uint8_t _pin;
};

#endif
