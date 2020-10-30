#include <analogWrite.h>
#include <WiFi.h>
#include <WiFiMulti.h>
#include <SocketIoClient.h>

WiFiMulti WiFiMulti;
SocketIoClient webSocket;

const char* SSID = "G6_9463";
const char* PASS = "UrteneEr100%Torre";
const char* IP = "192.168.43.182";
uint16_t PORT = 2520;
const bool DEBUG = true;

const byte gpio_25 = 25;

/**
 * Debugging tool
*/
template<typename T>
void log(T thingToLog, char* description=nullptr) {
    if (DEBUG) {
        Serial.print(description);
        Serial.println(thingToLog);
    }
}

/**
 *
*/
uint8_t str2int(const char* str) {
    String string(str);
    return string.toInt();
}

/**
 *
*/
class AnalogInput {
    public:
        AnalogInput(uint8_t pin, char* identifier) {
            _pin = pin;
            _identifier = identifier;
            pinMode(_pin, INPUT);
        }

        /**
         *
        */
        void sendData(const char* dataRequestData) {
            uint8_t request = str2int(dataRequestData);
            char buffer[17];
            itoa(analogRead(_pin), buffer, 17);
            log(request, "Request: ");
            log(buffer, "ITOA TEST: ");
            webSocket.emit(_identifier, buffer); // Change id in server script
        }

        /**
         *
        */
        char* getID() {
            return _identifier;
        }

    private:
        uint8_t _pin;
        char* _identifier;
};

/**
 * Every instance of AnalogOutput needs its own wrapper function to receive data from server, because
 * webSocket.on() only accepts static members.
*/
class AnalogOutput {
    public:
        AnalogOutput(uint8_t pin) {
            _pin = pin;
            pinMode(_pin, OUTPUT);
        }

        /**
         *
        */
        void power(const char* powerLevelData) {
            uint8_t powerLevel = str2int(powerLevelData);
            log(powerLevel, "Received power level: ");
            analogWrite(_pin, map(powerLevel, 0, 100, 0, 255));
        }

    private:
        uint8_t _pin;
};

AnalogInput waterLevelSensor(35, "waterLevelSensor");
AnalogInput soilHygrometer(36, "soilHygrometer");
AnalogOutput waterPump(33);
AnalogOutput light(34);

/**
 * Wrapper to avoid static error
*/
void getWaterLevelData(const char* dataRequestData, size_t length) {
    waterLevelSensor.sendData(dataRequestData);
}

/**
 * Wrapper to avoid static error
*/
void getSoilHygrometerData(const char* dataRequestData, size_t length) {
    soilHygrometer.sendData(dataRequestData);
}

/**
 * Wrapper to avoid static error
*/
void changeWaterPumpPower(const char* powerLevelData, size_t length) {
    log("Water pump - ");
    waterPump.power(powerLevelData);
}

/**
 * Wrapper to avoid static error
*/
void changeLightPower(const char* powerLevelData, size_t length) {
    log("Light - ");
    light.power(powerLevelData);
}

/**
 *
*/
void event(const char* payload, size_t length) { //Default event, what happens when you connect
    Serial.printf("got message: %s\n", payload);
}

void changeLEDState(const char * LEDStateData, size_t length) { //What happens when the ESP32 receives a instruction from the server (with variable) to change the LED
  Serial.printf("LED State: %s\n", LEDStateData); //First we print the data formated with the "printf" command
  Serial.println(LEDStateData); //Then we just print the LEDStateData which will be a int (0 og 1 so in reeality bool) that tells us what to do with the LED

  //Data conversion //We need som data processing to make this work
  String dataString(LEDStateData); //First we convert the const char array(*) to a string in Arduino (this makes thing easier)
  int LEDState = dataString.toInt(); //When we have a string we can use the built in Arduino function to convert to an integer

  Serial.print("This is the LED state in INT: "); //We print the final state
  Serial.println(LEDState);
  digitalWrite(gpio_25, LEDState);//We now use the varible to change the light (1 is on, 0 is off)
}

/**
 *
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
        delay(100);
    }
    Serial.print("\nConnected to WiFi successfully!\n");

    webSocket.on("clientConnected", event);
    webSocket.on("getWaterLevelData", getWaterLevelData);
    webSocket.on("getSoilHygrometerData", getSoilHygrometerData);
    webSocket.on("changeLightPower", changeLightPower);
    webSocket.on("changeWaterPumpPower", changeWaterPumpPower);
    webSocket.on("LEDStateChange", changeLEDState);

    webSocket.begin(IP, PORT);

    pinMode(gpio_25, OUTPUT);
}

/**
 *
*/
void loop() {
    webSocket.loop(); //Keeps the WebSocket connection running
  //DO NOT USE DELAY HERE, IT WILL INTERFER WITH WEBSOCKET OPERATIONS
  //TO MAKE TIMED EVENTS HERE USE THE millis() FUNCTION OR PUT TIMERS ON THE SERVER IN JAVASCRIPT
}