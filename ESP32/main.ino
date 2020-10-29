#include <analogWrite.h>
#include <WiFi.h>
#include <WiFiMulti.h>
#include <SocketIoClient.h>
#include "AnalogOutput.h"
#include "AnalogOutput.h"

WiFiMulti WiFiMulti;
SocketIoClient webSocket;
AnalogOutput waterPump(33/*pin*/);
AnalogOutput light(34/*pin*/);
AnalogInput waterLevelSensor(35/*pin*/, 0/*lower limit*/, 0/*upper limit*/);
AnalogInput soilHygrometer(36/*pin*/, 0/*lower limit*/, 0/*upper limit*/);

void event(const char* payload, size_t length) { //Default event, what happens when you connect
    Serial.printf("got message: %s\n", payload);
}

void changeLightBrightness(const char* powerLevelData, size_t length) {
    Serial.printf("Brightness: %s\n", powerLevelData);
    Serial.println(powerLevelData);

    String dataString(powerLevelData);
    uint8_t powerLevel = dataString.toInt();

    Serial.print("This is the brightness in INT [%]: ");
    Serial.println(powerLevel);
    light.on(powerLevel)
}

void waterPlants(const char* powerLevelData, size_t length) {
    Serial.printf("Power: %s\n", powerLevelData);
    Serial.println(powerLevelData);

    String dataString(powerLevelData);
    uint8_t powerLevel = dataString.toInt();

    Serial.print("This is the power in INT [%]: ");
    Serial.println(powerLevel);
    waterPump.on(powerLevel)
}

void dataRequest(const char * dataRequestData, size_t length) {
    Serial.printf("Datarequest Data: %s\n", dataRequestData);
    Serial.println(dataRequestData);

    String dataString(dataRequestData);
    int requestState = dataString.toInt();

    Serial.print("This is the Datarequest Data in INT: ");
    Serial.println(requestState);

    char buffer[10];
    char* id;

    switch (requestState) {
        case 0:
            itoa(waterLevelSensor.read(), buffer, 10);
            id = "WaterSensorData";
            break;
        case 1:
            itoa(soilHygrometer.read(), buffer, 10);
            id = "soilHygrometerData";
            break;
    }

    Serial.print("ITOA TEST: ");
    Serial.println(buffer);
    webSocket.emit(id, buffer); // Change id in server script
}

void setup() {
    Serial.begin(9600);

    Serial.setDebugOutput(true); //Set debug to true (during ESP32 booting)

    Serial.println();
    Serial.println();
    Serial.println();

    for (uint8_t t = 4; t > 0; t--) {
        Serial.printf("[SETUP] BOOT WAIT %d...\n", t);
        Serial.flush();
        delay(1000);
    }

    WiFiMulti.addAP("G6_9463", "UrteneEr100%Torre"); //Add a WiFi hotspot (addAP = add AccessPoint) (put your own network name and password in here)
    
    Serial.print("Not connected to wifi");
    while (WiFiMulti.run() != WL_CONNECTED) { // Wait for a successfull WiFi connection
        Serial.print('.');
        delay(100);
    }
    Serial.println("\nConnected to WiFi successfully!");

    //Here we declare all the different events the ESP32 should react to if the server tells it to.
    //a socket.emit("identifier", data) with any of the identifieres as defined below will make the socket call the functions in the arguments below
    webSocket.on("clientConnected", event); //For example, the socket.io server on node.js calls client.emit("clientConnected", ID, IP) Then this ESP32 will react with calling the event function
    webSocket.on("", changeLightBrightness);
    webSocket.on("", waterPlants);
    webSocket.on("dataRequest", dataRequest);

    webSocket.begin("192.168.43.182", 2520); //This starts the connection to the server with the ip-address/domainname and a port (unencrypted)
}

void loop() {
    webSocket.loop(); //Keeps the WebSocket connection running 
  //DO NOT USE DELAY HERE, IT WILL INTERFER WITH WEBSOCKET OPERATIONS
  //TO MAKE TIMED EVENTS HERE USE THE millis() FUNCTION OR PUT TIMERS ON THE SERVER IN JAVASCRIPT
}
