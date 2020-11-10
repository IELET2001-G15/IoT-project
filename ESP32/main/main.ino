#include <analogWrite.h>
#include <WiFi.h>
#include <WiFiMulti.h>
#include <SocketIoClient.h>

WiFiMulti WiFiMulti;
SocketIoClient webSocket;

const char* SSID = "G6_9463";
const char* PASS = "UrteneEr100%Torre";
const char* IP = "192.168.137.145";
uint16_t PORT = 2520;
const bool DEBUG = true;

/**
 * Debugging tool
 * @param thingToLog the data of arbitrary data type to display in console
 * @param description the optional description of the displayed data
*/
template<typename T>
void log(T thingToLog, char* description=nullptr) {
    if (DEBUG) {
        Serial.print(description);
        Serial.println(thingToLog);
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
         * @param dataRequestData the message sent from the server. Can be omitted
        */
        void sendData(const char* dataRequestData) {
            uint8_t request = atoi(dataRequestData);
            char buffer[17];
            itoa(analogRead(_pin), buffer, 17);
            log(request, "Request: ");
            log(buffer, "ITOA TEST: ");
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
*/
class AnalogOutput {
    public:
        AnalogOutput(uint8_t pin) {
            _pin = pin;
            pinMode(_pin, OUTPUT);
        }

        /**
         * Changes the power to the output to the value specified from server between 0 and 100%
         * @param powerLevelData the data from the server containing the power in text
        */
        void power(const char* powerLevelData) {
            uint8_t powerLevel = atoi(powerLevelData);
            log(powerLevel, "Received power level: ");
            analogWrite(_pin, map(powerLevel, 0, 100, 0, 255));
        }

    private:
        uint8_t _pin;
};

AnalogInput waterLevelSensor(35, "waterLevelSensor");
AnalogInput soilHygrometer(36, "soilHygrometer");
AnalogInput temperatureSensor(41, "temperatureSensor");
AnalogInput CO2Sensor(42, "CO2Sensor");
AnalogInput pHSensor(43, "pHSensor");
AnalogInput lightSensor(44, "lightSensor");
AnalogOutput waterPump(33);
AnalogOutput light(34);

/**
 * A series of wrapper functions to avoid static error. Handles event when server requests data, and redirects to the 
 * appropriate member function
 * @param dataRequestData the message from the server
 * @param length the size of the message
*/
void getWaterLevelData(const char* dataRequestData, size_t length) {
    waterLevelSensor.sendData(dataRequestData);
}

void getSoilHygrometerData(const char* dataRequestData, size_t length) {
    soilHygrometer.sendData(dataRequestData);
}

void getTemperatureData(const char* dataRequestData, size_t length) {
    temperatureSensor.sendData(dataRequestData);
}

void getCO2Data(const char* dataRequestData, size_t length) {
    CO2Sensor.sendData(dataRequestData);
}

void getPHData(const char* dataRequestData, size_t length) {
    pHSensor.sendData(dataRequestData);
}

void getLightData(const char* dataRequestData, size_t length) {
    lightSensor.sendData(dataRequestData);
}

/**
 * A series of wrapper functions to avoid static error. Handles event when server sends data, and redirects to the 
 * appropriate member function
 * @param powerLevelData the message from the server
 * @param length the size of the message
*/
void changeWaterPumpPower(const char* powerLevelData, size_t length) {
    log("Water pump - ");
    waterPump.power(powerLevelData);
}

void changeLightPower(const char* powerLevelData, size_t length) {
    log("Light - ");
    light.power(powerLevelData);
}

/**
 * Runs when ESP32 connects to server and displays client ID and IP
 * @param payload the message from containing client ID and IP
 * @param length the size of the message
*/
void event(const char* payload, size_t length) { //Default event, what happens when you connect
    Serial.printf("ID, IP: %s\n", payload);
}

/**
 * Handles initial setup, specifically connects to WiFi and server. Also contains all the events
 * the server can trigger
*/
void setup() {
    Serial.begin(9600);
    Serial.setDebugOutput(true); //Set debug to true (during ESP32 booting)
    Serial.print("\n\n\n");

    for (uint8_t t = 4; t > 0 && DEBUG; t--) {
        Serial.printf("[SETUP] BOOT WAIT %d...\n", t);
        Serial.flush();
        delay(1000);
    }

    WiFiMulti.addAP(SSID, PASS);
    
    Serial.print("Connecting to WiFi");
    while (WiFiMulti.run() != WL_CONNECTED) { // Wait for a successfull WiFi connection
        Serial.print('.');
        delay(50);
    }
    Serial.print("\nConnected to WiFi successfully!\n");

    webSocket.on("clientConnected", event);
    webSocket.on("getWaterLevelData", getWaterLevelData);

    webSocket.on("getSoilHygrometerData", getSoilHygrometerData);
    webSocket.on("getTemperatureData", getTemperatureData);
    webSocket.on("getCO2Data", getCO2Data);
    webSocket.on("getPHData", getPHData);
    webSocket.on("getLightData", getLightData);
    webSocket.on("changeLightPower", changeLightPower);
    webSocket.on("changeWaterPumpPower", changeWaterPumpPower);

    webSocket.begin(IP, PORT);
}

/**
 * Keeps the WebSocket connection running. DO NOT USE DELAY HERE, IT WILL INTERFER WITH 
 * WEBSOCKET OPERATIONS. TO MAKE TIMED EVENTS HERE USE THE millis() FUNCTION OR PUT TIMERS 
 * ON THE SERVER IN JAVASCRIPT
*/
void loop() {
    webSocket.loop(); 
}
