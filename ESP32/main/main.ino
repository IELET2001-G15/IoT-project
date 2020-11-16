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
#include <ESP32Servo.h>

AnalogIO waterLevelSensor(35, INPUT);
AnalogIO soilHygrometer(32, INPUT);
AnalogIO waterPump(33, OUTPUT);
AnalogIO light(25, OUTPUT);

WiFiMulti WiFiMulti;
SocketIoClient webSocket;
Adafruit_BME280 bme;
Servo servo;

const char* g_SSID = "G6_9463";
const char* g_PASS = "UrteneEr100%Torre";
const char* g_IP = "192.168.137.145";
const uint16_t g_PORT = 2520;

/**
 * Formats a number to a string. For example 1234 becomes "1234". Supports up to a four digit 
 * integer. Change buffer size to accomodate more digits
 * @param identifier the ID that the server will recognize
 * @param format the format of the string. For example "%d" for integer and "%f" for float
 * @param value the number to be formated as a string and sent to server
*/
void send(const char* identifier, const char* format, uint16_t value) {
    char buffer[33];
    sprintf(buffer, format, value);
    webSocket.emit(identifier, buffer);
}

/**
 * Handles event when server requests data and sends the data to server
 * @param identifier the message from the server
 * @param length the size of the message
*/
void sendData(const char* identifier, size_t length) {
    switch (identifier) {
        case "temperature": 
            send(identifier, "%f", bme.readTemperature()); break;
        case "airHumidity":
            send(identifier, "%f", bme.readHumidity()); break;
        case "soilHumidity":
            send(identifier, "%d", soilHygrometer.read()); break;
        case "waterLevel":
            send(identifier, "%d", waterLevelSensor.read()); break;
        default:
            send("temperature", "%f", bme.readTemperature());
            send("airHumidity", "%f", bme.readHumidity());
            send("soilHumidity", "%d", soilHygrometer.read());
            send("waterLevel", "%d", waterLevelSensor.read());
    }
}

/**
 * Handles event when server sends data and changes outputs accordingly
 * @param level the message from the server
 * @param length the size of the message
*/
void changeWaterPumpPower(const char* level, size_t length) {
    Serial.printf("Set to [%s]\n", level);
    waterPump.write(atoi(level));
}

void changeLightPower(const char* level, size_t length) {
    Serial.printf("Set to [%s]\n", level);
    light.write(atoi(level));
}

/**
 * Handles event when server asks for change in vent opening
 * @param angle the angle the servo-motor should turn
 * @param length the size of the message
*/
void changeVentAngle(const char* angle, size_t length) {
    Serial.printf("Set to [%s]\n", angle);
    servo.write(atoi(angle));
}

/**
 * Runs when a new client connects to server and displays client ID and IP
 * @param payload the message from containing client ID and IP
 * @param length the size of the message
*/
void clientConnected(const char* payload, size_t length) {
    Serial.printf("ID, IP: %s", payload);
}

/**
 * Handles initial setup, specifically connects to WiFi and server. Also contains all the events
 * the server can trigger
*/
void setup() {
    bme.begin();
    
    servo.setPeriodHertz(50);
    servo.attach(16, 1000, 2000);

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

    webSocket.on("clientConnected", clientConnected);
    webSocket.on("sendData", sendData);
    webSocket.on("changeLightPower", changeLightPower);
    webSocket.on("changeWaterPumpPower", changeWaterPumpPower);
    webSocket.on("changeVentAngle", changeVentAngle);

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
