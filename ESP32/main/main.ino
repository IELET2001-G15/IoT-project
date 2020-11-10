#include <analogWrite.h>
#include <WiFi.h>
#include <WiFiMulti.h>
#include <SocketIoClient.h>

WiFiMulti WiFiMulti;
SocketIoClient webSocket;

const char* g_SSID = "G6_9463";
const char* g_PASS = "UrteneEr100%Torre";
const char* g_IP = "192.168.137.145";
const uint16_t g_PORT = 2520;
const bool g_DEBUG = true;

/**
 * Debugging tool
 * @param firstThing the message of arbitrary data type to display in console first
 * @param secondThing the message of arbitrary data type to display in console after
*/
template<typename T1, typename T2>
void logEvent(T1 firstThing, T2 secondThing=nullptr) {
    if (g_DEBUG) {
        Serial.print(firstThing);
        Serial.println(secondThing);
    }
}

/**
 * Includes all functions and attibutes related to sensors and other input from pins.
 * Requires some dependencies to work as intended, specifically an instance of SocketIoClient 
 * called webSocket, and a wrapper function for each sendData() object to work around the problem
 * of using non-static members in webSocket.on() method
 * @param pin the pin the input is connected to
 * @param identifier the ID for the server to recognize each sensor
*/
class AnalogInput {
    public:
        AnalogInput(uint8_t pin, char* identifier) {
            _pin = pin;
            _identifier = identifier;
            pinMode(_pin, INPUT);
        }

        /**
         * Measures the sensor value and sends sensor data to the server as a number between 0 and 
         * 4095 as text. Needs a wrapper function for each sendData() object to work around the problem
         * of using non-static members in webSocket.on() method. This wrapper function have to have the
         * format: void wrapper(const char* data, size_t length) {instanceName.sendData(data);}
         * @param payload the message sent from the server. Can be omitted
        */
        void sendData() {
            char buffer[33];
            sprintf(buffer, "%d", analogRead(_pin));
            webSocket.emit(_identifier, buffer);
        }

    private:
        uint8_t _pin;
        char* _identifier;
};

/**
 * Includes all functions and attibutes related to output from pins. Requires some dependencies to 
 * work as intended, specifically the ESP32 analogWrite library, and a wrapper function for each 
 * power() object to work around the problem of using non-static members in webSocket.on() method
 * @param pin the pin the output is connected to
 * @param identifier the ID for the output
*/
class AnalogOutput {
    public:
        AnalogOutput(uint8_t pin, char* identifier) {
            _pin = pin;
            _identifier = identifier;
            pinMode(_pin, OUTPUT);
        }

        /**
         * Changes the power to the output to the value specified from server between 0 and 100%
         * @param powerLevelData the data from the server containing the power in text
        */
        void power(const char* powerLevelData) {
            uint8_t powerLevel = atoi(powerLevelData);
            logEvent("power level: ", powerLevel);
            analogWrite(_pin, map(powerLevel, 0, 100, 0, 255));
        }

    private:
        uint8_t _pin;
        char* _identifier;
};

AnalogInput waterLevelSensor(35, "waterLevelSensor");
AnalogInput soilHygrometer(36, "soilHygrometer");
//AnalogInput temperatureSensor(41, "temperatureSensor");
//AnalogInput CO2Sensor(42, "CO2Sensor");
//AnalogInput pHSensor(43, "pHSensor");
//AnalogInput lightSensor(44, "lightSensor");
AnalogOutput waterPump(33, "waterPump");
AnalogOutput light(34, "light");

/**
 * A series of wrapper functions to avoid static error. Handles event when server requests data, and redirects to the 
 * appropriate member function
 * @param payload the message from the server
 * @param length the size of the message
*/
void getWaterLevelData(const char* payload, size_t length) {
    waterLevelSensor.sendData();
}

void getSoilHygrometerData(const char* payload, size_t length) {
    soilHygrometer.sendData();
}
/*
void getTemperatureData(const char* payload, size_t length) {
    temperatureSensor.sendData();
}

void getCO2Data(const char* payload, size_t length) {
    CO2Sensor.sendData();
}

void getPHData(const char* payload, size_t length) {
    pHSensor.sendData();
}

void getLightData(const char* payload, size_t length) {
    lightSensor.sendData();
}
*/
/**
 * A series of wrapper functions to avoid static error. Handles event when server sends data, and redirects to the 
 * appropriate member function
 * @param powerLevelData the message from the server
 * @param length the size of the message
*/
void changeWaterPumpPower(const char* powerLevelData, size_t length) {
    waterPump.power(powerLevelData);
}

void changeLightPower(const char* powerLevelData, size_t length) {
    light.power(powerLevelData);
}

/**
 * Runs when ESP32 connects to server and displays client ID and IP
 * @param payload the message from containing client ID and IP
 * @param length the size of the message
*/
void event(const char* payload, size_t length) {
    logEvent(payload, "ID, IP: ");
}

/**
 * Handles initial setup, specifically connects to WiFi and server. Also contains all the events
 * the server can trigger
*/
void setup() {
    Serial.begin(9600);
    Serial.setDebugOutput(true); //Set debug to true (during ESP32 booting)
    Serial.print("\n\n\n");

    for (uint8_t t = 4; t > 0 && g_DEBUG; t--) {
        Serial.printf("[SETUP] BOOT WAIT %d...\n", t);
        Serial.flush();
        delay(1000);
    }

    WiFiMulti.addAP(g_SSID, g_PASS);
    
    Serial.print("Connecting to WiFi");
    while (WiFiMulti.run() != WL_CONNECTED) { // Wait for a successfull WiFi connection
        Serial.print('.');
        delay(50);
    }
    Serial.print("\nConnected to WiFi successfully!\n");

    webSocket.on("clientConnected", event);
    webSocket.on("getWaterLevelData", getWaterLevelData);
    webSocket.on("getSoilHygrometerData", getSoilHygrometerData);
    //webSocket.on("getTemperatureData", getTemperatureData);
    //webSocket.on("getCO2Data", getCO2Data);
    //webSocket.on("getPHData", getPHData);
    //webSocket.on("getLightData", getLightData);
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
