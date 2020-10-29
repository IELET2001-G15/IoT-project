#include "AnalogInput.h"

AnalogInput::AnalogInput(uint8_t pin, int16_t lowerLimit, int16_t upperLimit, char* identifier) {
    _pin = pin;
    _lowerLimit = lowerLimit;
    _upperLimit = upperLimit;
    _identifier = identifier;
    pinMode(_pin, INPUT);
}

uint16_t AnalogInput::readRaw() {
    return analogRead(_pin);
}

float AnalogInput::read() {
    return float(readRaw()); // Calculate some value based on lower and upper limits
}

void AnalogInput::dataRequest(const char* dataRequestData, size_t length) {
    Serial.printf("Datarequest Data: %s\n", dataRequestData);
    Serial.println(dataRequestData);

    String dataString(dataRequestData);
    int requestState = dataString.toInt();

    Serial.print("This is the Datarequest Data in INT: ");
    Serial.println(requestState);

    char buffer[10];
    itoa(read(), buffer, 10);
    Serial.print("ITOA TEST: ");
    Serial.println(buffer);

    webSocket.emit(_dentifier, buffer); // Change id in server script
}
