/**
 * main.ino - Main file for sending and receiving data between web server and ESP32
 * Created by Espen Holsen, November 14, 2020.
 * Released into the public domain.
*/

#include "AnalogIO.h"
#include <WiFi.h>
#include <WiFiMulti.h>
#include <SocketIoClient.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

AnalogIO waterLevelSensor(35, INPUT);
AnalogIO soilHygrometer(32, INPUT);
AnalogIO waterPump(33, OUTPUT);
AnalogIO light(25, OUTPUT);

Adafruit_BME280 bme;
WiFiMulti WiFiMulti;
SocketIoClient webSocket;

const char* g_SSID = "G6_9463";
const char* g_PASS = "UrteneEr100%Torre";
const char* g_IP = "192.168.137.145";
const uint16_t g_PORT = 2520;

/**
 * Formats a number to a string. For example 1234 becomes "1234". Supports up to a four digit 
 * number. Change buffer size to accomodate more digits
 * @param identifier the ID that the server will recognize
 * @param format the format of the string. For example 
 * @param number the number to convert to string
 * @return buffer the number formated as a string
*/
void send(const char* identifier, const char* format, uint16_t number) {
    char buffer[33];
    sprintf(buffer, format, number);
    webSocket.emit(identifier, buffer);
}

/**
 * Handles event when server requests data and sends the data to server
 * @param payload the message from the server
 * @param length the size of the message
*/
void sendWaterLevelData(const char* payload, size_t length) {
    send("waterLevelSensor", "%d", waterLevelSensor.read());
}

void sendSoilHygrometerData(const char* payload, size_t length) {
    send("soilHygrometer", "%d", soilHygrometer.read());
}

void sendTemperatureData(const char* payload, size_t length) {
    send("temperatureSensor", "%d", bme.readTemperature());
}

void sendAirHygrometerData(const char* payload, size_t length) {
    send("temperatureSensor", "%d", bme.readHumidity());
}

/**
 * Handles event when server sends data and changes outputs accordingly
 * @param powerLevelData the message from the server
 * @param length the size of the message
*/
void changeWaterPumpPower(const char* powerLevelData, size_t length) {
    Serial.printf("Set to [%s]\n", powerLevelData);
    waterPump.write(atoi(powerLevelData));
}

void changeLightPower(const char* powerLevelData, size_t length) {
    Serial.printf("Set to [%s]\n", powerLevelData);
    light.write(atoi(powerLevelData));
}

/**
 * Runs when a new client connects to server and displays client ID and IP
 * @param payload the message from containing client ID and IP
 * @param length the size of the message
*/
void event(const char* payload, size_t length) {
    Serial.printf("ID, IP: %s", payload);
}

/**
 * Handles initial setup, specifically connects to WiFi and server. Also contains all the events
 * the server can trigger
*/
void setup() {
    bme.begin(); 

    Serial.begin(9600);
    Serial.setDebugOutput(true); //Set debug to true (during ESP32 booting)
    Serial.print("\n\n\n");

    for (uint8_t t = 4; t > 0; t--) {
        Serial.printf("[SETUP] BOOT WAIT %d...\n", t);
        Serial.flush();
        delay(1000);
    }

    WiFiMulti.addAP(g_SSID, g_PASS);
    
    Serial.print("Connecting to WiFi");
    while (WiFiMulti.run() != WL_CONNECTED) Serial.print('.');
    Serial.print("\nConnected to WiFi successfully!\n\n\n");

    webSocket.on("clientConnected", event);
    webSocket.on("getWaterLevelData", sendWaterLevelData);
    webSocket.on("getSoilHygrometerData", sendSoilHygrometerData);
    webSocket.on("changeLightPower", changeLightPower);
    webSocket.on("changeWaterPumpPower", changeWaterPumpPower);

    webSocket.begin(g_IP, g_PORT);
}

/**
 * Keeps the WebSocket connection running. DO NOT USE DELAY HERE, IT WILL INTERFER WITH 
 * WEBSOCKET OPERATIONS. TO MAKE TIMED EVENTS HERE USE THE millis() FUNCTION OR PUT TIMERS 
 * ON THE SERVER IN JAVASCRIPT
*/
void loop() {
    webSocket.loop(); 
}
