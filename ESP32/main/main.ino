#include <WiFi.h>
#include <WiFiMulti.h>
#include <SocketIoClient.h>
#include <Wire.h>
#include "AnalogIO.h"

const char* g_SSID = "G6_9463";
const char* g_PASS = "UrteneEr100%Torre";
const char* g_IP = "192.168.137.145";
const uint16_t g_PORT = 2520;

WiFiMulti WiFiMulti;
SocketIoClient webSocket;

AnalogIO waterLevelSensor(32, INPUT);
AnalogIO soilHygrometer(33, INPUT);
AnalogIO waterPump(34, OUTPUT);
AnalogIO light(35, OUTPUT);

/**
 * Formats a number to a string. For example 1234 becomes "1234". Supports up to a four digit 
 * number. Change buffer size to accomodate more digits
 * @param number the number to convert to string
 * @return buffer the number formated as a string
*/
char* int2str(uint16_t integer) {
    char buffer[33];
    sprintf(buffer, "%d", integer);
    return buffer;
}

/**
 * Converts a string to a number. For example "1234" becomes 1234. Also logs the value in 
 * serial monitor for debugging purposes
 * @param string the string to convert to number
 * @return integer the number 
*/
uint8_t str2int(const char* string) {
    uint8_t integer;
    sscanf(string, "%d", integer);
    Serial.printf("Set to [%d]", integer);
    return integer;
}

/**
 * Handles event when server requests data and sends the data to server
 * @param payload the message from the server
 * @param length the size of the message
*/
void sendWaterLevelData(const char* payload, size_t length) {
    webSocket.emit("waterLevelSensor", int2str(waterLevelSensor.read()));
}

void sendSoilHygrometerData(const char* payload, size_t length) {
    webSocket.emit("soilHygrometer", int2str(soilHygrometer.read()));
}

/**
 * Handles event when server sends data and changes outputs accordingly
 * @param powerLevelData the message from the server
 * @param length the size of the message
*/
void changeWaterPumpPower(const char* powerLevelData, size_t length) {
    waterPump.write(str2int(powerLevelData));
}

void changeLightPower(const char* powerLevelData, size_t length) {
    light.write(str2int(powerLevelData));
}

/**
 * Runs when ESP32 connects to server and displays client ID and IP
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
