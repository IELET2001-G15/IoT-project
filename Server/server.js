var fs = require('fs');
var http = require('http');
var express = require('express');
var app = express();

var serverPort = 2520;

var server = http.createServer(app);
var io = require('socket.io').listen(server);

server.listen(serverPort, function() {
    console.log('listening on *:' + serverPort);
});


io.on('connection', function(socket) { //This is the server part of the "what happens when we first connect" function. Everytime a user connects a instance of this is set up for the user privatley
    var clientID = socket.id;
    var client = io.sockets.connected[clientID];
    var clientIPRAW = client.request.connection.remoteAddress;
    var IPArr = clientIPRAW.split(":",4);

    console.log('a user connected');
    console.log("User ID: " + clientID);
    console.log("User IP: " + IPArr[3]);

    io.emit("clientConnected", clientID, IPArr[3]);

    var timer;

    /**
     * Handles event when client disconnects. Stops data requests by clearing timer
     */
    client.on('disconnect', function() {
        clearInterval(timer);
        console.log("user " + clientID + " disconnected, stopping timers if any");
    });

    /**
     * Handles event when a web page wants to change light brightness on ESP32.
     * Forwards power to ESP32
     * @param power the power which is set to the light in bits
     */
    socket.on('lightPower', function(power) {
        io.emit('changeLightPower', power);
        console.log('user ' + clientID + ' changed the light power to [%]: ' + power);
    });

    /**
     * Handles event when a web page wants to change water pump power on ESP32. 
     * Forwards power to ESP32
     * @param power the power which is set to the water pump in bits
     */
    socket.on('waterPumpPower', function(power) {
        io.emit('changeWaterPumpPower', power);
        console.log('user ' + clientID + ' changed the water pump power to [%]: ' + power);
    });

    /**
     * Handles event when a web page wants to change vent opening on ESP32.
     * Forwards angle to ESP32
     * @param angle the angle which the servo motor turns in degrees
     */
    socket.on('ventAngle', function(angle) {
        io.emit('changeVentAngle', angle);
        console.log('user ' + clientID + ' changed the vent angle to [deg]: ' + angle);
    });

    /**
     * Handles event when web page requests sensor data from ESP32.
     * Clears previous timers to avoid more than one request interval at once and 
     * forwards the request to ESP32
     * @param request the sensor that is requested. "all" if all sensors
     * @param interval the time between each consecutive request
     */
    socket.on('requestDataFromBoard', function(request, interval) { // Receives from webpage
        clearInterval(timer);
        timer = setInterval(function() {
                    io.emit('sendData', request);
                }, interval);
        console.log('user ' + clientID + ' requested ' + request + ' data with interval [ms]: ' + interval);
    });

    /**
     * Handles event when web page does not want data anymore. Stops data requests 
     * by clearing timer
     */
    socket.on('stopDataFromBoard', function() {
        clearInterval(timer);
        console.log('user ' + clientID + ' cleared data request interval');
    });

    /**
     * Handles a series of events when ESP32 sends its sensor data.
     * Forwards the data to the web page
     * @param data the data sent from the ESP32
     */
    socket.on('waterLevel', function(data) {
        io.emit('pushWaterLevel', data);
        console.log('user ' + clientID + ' gained the data: ' + data);
    });

    socket.on('soilHumidity', function(data) {
        io.emit('pushSoilHumidity', data);
        console.log('user ' + clientID + ' gained the data: ' + data);
    });

    socket.on('airHumidity', function(data) {
        io.emit('pushAirHumidity', data);
        console.log('user ' + clientID + ' gained the data: ' + data);
    });

    socket.on('temperature', function(data) {
        io.emit('pushTemperature', data);
        console.log('user ' + clientID + ' gained the data: ' + data);
    });

    socket.on('CO2', function(data) {
        io.emit('pushCO2', data);
        console.log('user ' + clientID + ' gained the data: ' + data);
    });

    socket.on('pH', function(data) {
        io.emit('pushpH', data);
        console.log('user ' + clientID + ' gained the data: ' + data);
    });

    socket.on('lux', function(data) {
        io.emit('pushLux', data);
        console.log('user ' + clientID + ' gained the data: ' + data);
    });
});
