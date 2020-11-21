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
#include <SparkFun_VEML6030_Ambient_Light_Sensor.h>
#include <ESP32Servo.h>

AnalogIO waterLevel(34, INPUT);
AnalogIO soilHumidity(35, INPUT);
AnalogIO waterPump(33, OUTPUT);
AnalogIO light(25, OUTPUT);
SparkFun_Ambient_Light veml(0x48);
Adafruit_BME280 bme;
Servo servo;

WiFiMulti WiFiMulti;
SocketIoClient webSocket;

const char* g_SSID = "G6_9463";
const char* g_PASS = "UrteneEr100%Torre";
const char* g_IP = "192.168.137.151";
const uint16_t g_PORT = 2520;

/**
 * Formats a number to a string. For example 1234 becomes "1234". Change buffer size to 
 * accomodate more digits
 * @param identifier the ID that the server will recognize
 * @param format the format of the string. For example "%d" for integer and "%f" for float
 * @param value the number to be formated as a string and sent to server
 */
template<typename T>
void send(const char* identifier, const char* format, T value) {
    char buffer[10];
    sprintf(buffer, format, value);
    webSocket.emit(identifier, buffer);
}

/**
 * Handles event when server requests data and sends the data to server
 * @param identifier the specific sensor that is requested. "all" if all sensors are requested
 * @param length the size of the message
 */
void sendData(const char* identifier, size_t length) {
    if (!strcmp(identifier, "temperature")) {
        send("temperature", "%f", bme.readTemperature());
    } else if (!strcmp(identifier, "airHumidity")) {
        send("airHumidity", "%f", bme.readHumidity());
    } else if (!strcmp(identifier, "soilHumidity")) {
        send("soilHumidity", "%d", soilHumidity.read());
    } else if (!strcmp(identifier, "waterLevel")) {
        send("waterLevel", "%d", waterLevel.read());
    } else if (!strcmp(identifier, "lux")) {
        send("lux", "%d", veml.readLight());
    } else if (!strcmp(identifier, "all")) {
        send("temperature", "%4.2f", bme.readTemperature());
        send("airHumidity", "%4.2f", bme.readHumidity());
        send("soilHumidity", "%d", soilHumidity.read());
        send("waterLevel", "%d", waterLevel.read());
        send("lux", "%d", veml.readLight());
    } else {
        Serial.printf("[ERROR] Could not find sensor with identifier: %s\n", identifier);
    }
}

/**
 * Handles event when server sends data and changes outputs accordingly
 * @param level the message from the server
 * @param length the size of the message
 */
void changeWaterPumpPower(const char* level, size_t length) {
    Serial.printf("[OUTPUT] Set water pump to [bits]: %s\n", level);
    servo.write(atoi(level));
}

void changeLightPower(const char* level, size_t length) {
    Serial.printf("[OUTPUT] Set light to [bits]: %s\n", level);
    light.write(atoi(level));
}

/**
 * Handles event when server asks for change in vent angle
 * @param angle the angle the servo-motor should turn
 * @param length the size of the message
 */
void changeVentAngle(const char* angle, size_t length) {
    Serial.printf("[OUTPUT] Set servo to [deg]: %s\n", angle);
    servo.write(atoi(angle));
}

/**
 * Handles event when a new client connects to server, and displays client ID and IP
 * @param payload the message from containing client ID and IP
 * @param length the size of the message
 */
void clientConnected(const char* payload, size_t length) {
    Serial.printf("ID, IP: %s\n", payload);
}

/**
 * Handles initial setup, specifically connects to WiFi and server. Also contains all the events
 * the server can trigger
 */
void setup() {
    Serial.begin(9600);
    Serial.setDebugOutput(true); //Set debug to true (during ESP32 booting)

    for (uint8_t t = 4; t > 0; t--) {
        Serial.printf("[SETUP] BOOT WAIT %d...\n", t);
        Serial.flush();
        delay(1000);
    }

    WiFiMulti.addAP(g_SSID, g_PASS);
    
    Serial.print("[SETUP] Connecting to WiFi");
    while (WiFiMulti.run() != WL_CONNECTED) Serial.print('.');
    Serial.printf("\n[SETUP] Connected to: %s\n", g_SSID);

    webSocket.on("clientConnected", clientConnected);
    webSocket.on("sendData", sendData);
    webSocket.on("changeLightPower", changeLightPower);
    webSocket.on("changeWaterPumpPower", changeWaterPumpPower);
    webSocket.on("changeVentAngle", changeVentAngle);

    webSocket.begin(g_IP, g_PORT);

    Wire.begin();
    bme.begin();
    veml.begin();
    veml.setGain(0.125);
    veml.setIntegTime(100);
    servo.setPeriodHertz(50);
    servo.attach(16, 1000, 2000);
}

/**
 * Keeps the WebSocket connection running. DO NOT USE DELAY HERE, IT WILL INTERFER WITH 
 * WEBSOCKET OPERATIONS. TO MAKE TIMED EVENTS HERE USE THE millis() FUNCTION OR PUT TIMERS 
 * ON THE SERVER IN JAVASCRIPT
 */
void loop() {
    webSocket.loop(); 
}
