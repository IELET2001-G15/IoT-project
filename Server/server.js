var fs = require('fs');
var http = require('http');
var express = require('express');
var app = express(); //These 4 first variables can be viewed as importing 3 libraries we need for the server and then declaring the objects we need to use them (just like Arduino)

var serverPort = 2520; //We set the port the server will run on. This can be anything you want, as long as it does not interfere with other software

var server = http.createServer(app); //We declare and create a http server (this because the WebSocket protocol formally is a "upgraded" version of the HTTP protocol)
var io = require('socket.io').listen(server); //Then we start the WebSocket protocol on top of the HTTP server

//HTTP Server
server.listen(serverPort, function(){ //Here we tell the server to start listening (open) on the port we earlier defined
    console.log('listening on *:' + serverPort);
});


io.on('connection', function(socket){ //This is the server part of the "what happens when we first connect" function. Everytime a user connects a instance of this is set up for the user privatley
    var clientID = socket.id; //This is the actual clientID in alphanumeric characters (a string variable)
    var client = io.sockets.connected[clientID]; //This is the client object, each client has its own object (again like in Arduino declaring a object)
    var clientIPRAW = client.request.connection.remoteAddress; //Fetch the IP-address of the client that just connected
    var IPArr = clientIPRAW.split(":",4); //Split it, which is to say reformat the fetched data

    console.log('a user connected'); //The server print this message
    console.log("User ID: " + clientID);
    console.log("User IP: " + IPArr[3]); //Print out the formated IP-address

    io.emit("clientConnected", clientID, IPArr[3]); //Now we can use our custom defined "on connection" function to tell the client its ID and IP-address

    var interval = 1000;
    var timer;

    client.on('disconnect', function() {
        clearInterval(timer);
        console.log("user " + clientID + " disconnected, stopping timers if any");
    });

    /**#####################################################################################
     * Change output on ESP
     */

    socket.on('lightPower', function(power) { // Receives from webpage
        io.emit('changeLightPower', power);   // Sends to ESP
        console.log('user ' + clientID + ' changed the light power to [%]: ' + power);
    });

    socket.on('waterPumpPower', function(power) {
        io.emit('changeWaterPumpPower', power);
        console.log('user ' + clientID + ' changed the water pump power to [%]: ' + power);
    });

    socket.on('ventAngle', function(angle) {
        io.emit('changeVentAngle', angle);
        console.log('user ' + clientID + ' changed the vent angle to [deg]: ' + angle);
    });

    /**######################################################################################
     * Request data from ESP
     */

    socket.on('requestDataFromBoard', function(request) { // Receives from webpage
        clearInterval(timer);
        timer = setInterval(function() {
                    io.emit('sendData', request);
                }, interval);
        console.log('user ' + clientID + ' requested ' + request + ' data with interval [ms]: ' + interval);
    });

    socket.on('stopDataFromBoard', function() {
        clearInterval(timer);
        console.log('user ' + clientID + ' cleared data request interval');
    });

    socket.on('changeInterval', function(newInterval) {
        interval = newInterval;
        console.log('user ' + clientID + ' changed the interval to [ms]: ' + newInterval);
    });


    /**#####################################################################################
     * Send data to web page
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
