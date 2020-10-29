#include <analogWrite.h>
#include <WiFi.h>
#include <WiFiMulti.h>
#include <SocketIoClient.h>

WiFiMulti WiFiMulti;
SocketIoClient webSocket;

/**
 * 
*/
class AnalogInput {
    public:
        AnalogInput(uint8_t pin, int16_t lowerLimit, int16_t upperLimit, char* identifier) {
            _pin = pin;
            _lowerLimit = lowerLimit;
            _upperLimit = upperLimit;
            _identifier = identifier;
            pinMode(_pin, INPUT);
        }

        /**
         * Reads
        */
        float read() {
            return float(_upperLimit - _lowerLimit)/4095.0*float(analogRead(_pin)) + _lowerLimit;
        }

        /**
         * 
        */
        void sendData() {
            char buffer[33];
            itoa(read(), buffer, 33);
            Serial.print("ITOA TEST: ");
            Serial.println(buffer);
            webSocket.emit(_identifier, buffer); // Change id in server script
        }

    private:
        uint8_t _pin;
        int16_t _lowerLimit;
        int16_t _upperLimit;
        char* _identifier;
};

class AnalogOutput {
    public:
        AnalogOutput(uint8_t pin) {
            _pin = pin;
            pinMode(_pin, OUTPUT);
        }

        /**
         * 
        */
        void on(uint8_t level = 100) {
            analogWrite(_pin, map(level, 0, 100, 0, 255));
        }

        /**
         * 
        */
        void off() {
            on(0);
        }

        /**
         * 
        */
        void receiveData(const char* powerLevelData, size_t length) {
            Serial.printf("Received: %s\n", powerLevelData);

            String dataString(powerLevelData);
            uint8_t powerLevel = dataString.toInt();

            Serial.print("This is the received in INT [%]: ");
            Serial.println(powerLevel);
            on(powerLevel);
        }

    private:
        uint8_t _pin;
};


AnalogOutput waterPump(33);
AnalogOutput light(34);
AnalogInput waterLevelSensor(35, 0, 0, "");
AnalogInput soilHygrometer(36, 0, 0, "");

/**
 * 
*/
void dataRequest(const char* dataRequestData, size_t length) {
    Serial.printf("Datarequest Data: %s\n", DataRequestData);

    String dataString(dataRequestData);
    int requestState = dataString.toInt();

    Serial.print("This is the Datarequest Data in INT: ");
    Serial.println(requestState);

    switch (requestState) {
        case 0:
            waterLevelSensor.sendData();
            break;
        case 1:
            soilHygrometer.sendData();
    }  
}

/**
 * Wrapper to avoid static error
*/
void changeLightBrightness(const char* dataRequestData, size_t length) {
    light.receiveData(dataRequestData, length);
}

/**
 * Wrapper to avoid static error
*/
void waterPlants(const char* dataRequestData, size_t length) {
    waterPump.receiveData(dataRequestData, length);
}

/**
 * 
*/
void event(const char* payload, size_t length) { //Default event, what happens when you connect
    Serial.printf("got message: %s\n", payload);
}

void setup() {
    Serial.begin(9600);
    Serial.setDebugOutput(true); //Set debug to true (during ESP32 booting)
    Serial.print("\n\n\n");

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
    webSocket.on("dataRequest", dataRequest);
    webSocket.on("", changeLightBrightness);
    webSocket.on("", waterPlants);

    webSocket.begin("192.168.43.182", 2520); //This starts the connection to the server with the ip-address/domainname and a port (unencrypted)
}

void loop() {
    webSocket.loop(); //Keeps the WebSocket connection running 
  //DO NOT USE DELAY HERE, IT WILL INTERFER WITH WEBSOCKET OPERATIONS
  //TO MAKE TIMED EVENTS HERE USE THE millis() FUNCTION OR PUT TIMERS ON THE SERVER IN JAVASCRIPT
}
