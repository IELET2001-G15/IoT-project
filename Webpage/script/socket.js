var socket = io.connect('192.168.137.145:2520', {secure: false}); //This line declares a socket.io object to var "socket" and connects to the server (change the IP-address and port to your own)
//The "secure: false" tells if the connection will be encrypted or not. Since we will not encrypt our connections, this is false.

//Socket.io has several functions. The .on function refers to what will happen when the client receive a call called 'connect' from the server
//View it as calling a function remotley. The server tells the client it wants to call this function with no arguments.
socket.on('connect',function() { //When you connect to the server (and it works) call this function
    console.log('Client has connected to the server!'); //The client prints this message
}); //The 'connect' function/identifier is the standard procedure. To make something more we have to make it ourselves

socket.on('clientConnected', function(id, ip) {
    console.log('Client recevied ID: ' + id);
    console.log("Client IP: " + ip);
});

//################################################################################################

socket.on('graphWaterLevel', function(data) { //Received data from the server who is forwarding it to us from the ESP32
    console.log('Water level data was received: ' + data);
    waterLevelArray.push(Number(data)); //This pushes data to the array that stores all the chart data
    myLineChart.update();
    updateTime();
    printDataValues();
});

socket.on('graphSoilHumidity', function(data) { //Received data from the server who is forwarding it to us from the ESP32
    console.log('Soil humidity data was received: ' + data);
    soilHumidityArray.push(Number(data)); //This pushes data to the array that stores all the chart data
    myLineChart.update();
    updateTime();
    printDataValues();
});

socket.on('graphAirHumidity', function(data) { //Received data from the server who is forwarding it to us from the ESP32
    console.log('Air humidity data was received: ' + data);
    airHumidityArray.push(Number(data)); //This pushes data to the array that stores all the chart data
    myLineChart.update();
    updateTime();
    printDataValues();
});

socket.on('graphTemperature', function(data) { //Received data from the server who is forwarding it to us from the ESP32
    console.log('Temperature data was received: ' + data);
    temperatureArray.push(Number(data)); //This pushes data to the array that stores all the chart data
    myLineChart.update();
    updateTime();
    printDataValues();
});

socket.on('graphCO2', function(data) { //Received data from the server who is forwarding it to us from the ESP32
    console.log('CO2 data was received: ' + data);
    CO2Array.push(Number(data)); //This pushes data to the array that stores all the chart data
    myLineChart.update();
    updateTime();
    printDataValues();
});

socket.on('graphpH', function(data) { //Received data from the server who is forwarding it to us from the ESP32
    console.log('pH data was received: ' + data);
    pHArray.push(Number(data)); //This pushes data to the array that stores all the chart data
    myLineChart.update();
    updateTime();
    printDataValues();
});

socket.on('graphLux', function(data) { //Received data from the server who is forwarding it to us from the ESP32
    console.log('Lux data was received: ' + data);
    luxArray.push(Number(data)); //This pushes data to the array that stores all the chart data
    myLineChart.update();
    updateTime();
    printDataValues();
});

//###################################################################################

//In this function (which is essentially built up the same as a void function in Arduino) we want to send something to the server
//For this we use the other important Socket.io function, .emit(arg). Here we are telling our socket object so call the "changeLEDState" function
//on the server with the "state" argument. By calling the function on the server we mean that we send data to the server that tells it to do something
function lightPower(power) {
    socket.emit('lightPower', power);
    console.log('lightPower was called with power [bits]: ' + power);
}

function waterPumpPower(power) {
    socket.emit('waterPumpPower', power);
    console.log('waterPumpPower was called with power [bits]: ' + power);
}

function ventAngle(angle) {
    socket.emit('ventAngle', angle);
    console.log('ventAngle was called with angle [deg]: ' + angle);
}

//This function also emits something to the server. But in this case we want something a little bit more complex to happen.
//Since Arduino has a limited amount of timers, and using millis can be annoying, we have the possibilties of handing that task over to JavaScript on Node.js
//The function we are calling here will tell the server to set up a JavaScript timer, which then will periodically send a message to the ESP32 asking for data.
//Since the ESP32 easily can react to such a request it sends the data with no problems, and with no timers in use.
//This means we dont have to use the delay() function or the millis() function in Arduino, we can just let Node and JavaScript fix the tracking of time for us
//This is the function that will make the ESP32 transmit data to the server, and not the other way around

function requestDataFromBoard(request) {
    socket.emit('requestDataFromBoard', request);
    console.log('requestDataFromBoard was called with request: ' + request);
}

function stopDataFromBoard() { //Tells the server to stop all timers so that data is no longer sent from the ESP32 to the webpage
    socket.emit('stopDataFromBoard'); //Here we tell the server to call the function "stopDataFromBoard"
    console.log("stopDataFromBoard was called");
}

function changeInterval(interval) {
    socket.emit('changeInterval', interval);
    console.log('changeInterval was called with interval [ms]: ' + interval);
}
