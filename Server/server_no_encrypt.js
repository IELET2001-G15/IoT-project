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
    console.log('a user connected'); //The server print this message

    //Client ID needs to be fetched
    var clientID = socket.id; //This is the actual clientID in alphanumeric characters (a string variable)
    var client = io.sockets.connected[clientID]; //This is the client object, each client has its own object (again like in Arduino declaring a object)
    //client.emit("clientConnected", clientID); THIS SHOULDNT BE HERE I THINK
    console.log("User ID: " + clientID);

    //Client IP
    var clientIPRAW = client.request.connection.remoteAddress; //Fetch the IP-address of the client that just connected

    var IPArr = clientIPRAW.split(":",4); //Split it, which is to say reformat the fetched data
    console.log("User IP: " + IPArr[3]); //Print out the formated IP-address

    io.emit("clientConnected", clientID, IPArr[3]); //Now we can use our custom defined "on connection" function to tell the client its ID and IP-address

    //Disconnect protocol
    client.on('disconnect', function(){ //This function is called for a client when the client is disconnected. The server can then do something even tough the client is disconnected
        console.log("user " + clientID + " disconnected, stopping timers if any");

        for (var i = 0; i < timers.length; i++) {//clear timer if user disconnects
            clearTimeout(timers[i]); //Cleartimer is the same as stopping the timer, in this case we clear all possible timers previously set
        }
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

    //Change states (general user defined functions)
    socket.on('changeLEDState', function(state) { //This server function constantly checks if a client (webpage) calls its
        //If the webpage calles it it will us the "io.emit" (to send to alle clients) and not "client.emit" to only send to one client
        //In this way, when we send it to call clients, the ESP32 will get the message. It is an easy solution which can be made better

        io.emit('LEDStateChange', state); //This is the actual socket.io emit function
        console.log('user ' + clientID + ' changed the LED state to: ' + state);
        
    });

    /**#####################################################################################
     *
     */

    var timers = []; //Stores all our timers
    //Read data from board section

    /**######################################################################################
     * Request data from ESP
     */

    socket.on('waterLevelData', function(interval) { // Receives from webpage
        console.log('user ' + clientID + ' requested data with interval (ms): ' + interval);
        if(interval > 99) {
            timers.push(
                setInterval(function(){
                    io.emit('getWaterLevelData', 0); // Sends to ESP
                }, interval)
            );
        } else {
            console.log("too short time interval");
        }
    });

    socket.on('stopDataFromBoard', function() { //This function stops all the timers set by a user so that data will no longer be sent to the webpage
        console.log('user ' + clientID + ' cleared data request interval');
        for (var i = 0; i < timers.length; i++) {//For loop to clear all set timers
            clearTimeout(timers[i]); //Cleartimer is the same as stopping the timer, in this case we clear all possible timers previously set
        }
    });


    /**#####################################################################################
     * Send data to web page
     */

    socket.on('waterLevelSensor', function(data) { //This is function that actually receives the data. The earlier one only starts the function.
        io.emit('graphWaterLevelSensor', data); //Everytime a "dataFromBoard" tag (with data) is sent to the server, "data" tag with the actual data is sent to all clients
        io.emit('graphTimers', timers); //Everytime a "dataFromBoard" tag (with data) is sent to the server, "data" tag with the actual data is sent to all clients
        //This means the webbrowser will receive the data, and can then graph it or similar.
        console.log('user ' + clientID + ' gained the data: ' + data);
    });




});






/*

socket.on('soilHygrometerData', function(interval) {
    console.log('user ' + clientID + ' requested data with interval (ms): ' + interval);
    if(interval > 99) {
        timers.push(
            setInterval(function(){
                io.emit('getSoilHygrometerData', 0);
            }, interval)
        );
    } else {
        console.log("too short timeintervall");
    }
});

socket.on('temperatureData', function(interval) {
    console.log('user ' + clientID + ' requested data with interval (ms): ' + interval);
    if(interval > 99) {
        timers.push(
            setInterval(function(){
                io.emit('getTemperatureData', 0);
            }, interval)
        );
    } else {
        console.log("too short timeintervall");
    }
});

socket.on('co2Data', function(interval) {
    console.log('user ' + clientID + ' requested data with interval (ms): ' + interval);
    if(interval > 99) {
        timers.push(
            setInterval(function(){
                io.emit('getCO2Data', 0);
            }, interval)
        );
    } else {
        console.log("too short timeintervall");
    }
});

socket.on('pHData', function(interval) {
    console.log('user ' + clientID + ' requested data with interval (ms): ' + interval);
    if(interval > 99) {
        timers.push(
            setInterval(function(){
                io.emit('getPHData', 0);
            }, interval)
        );
    } else {
        console.log("too short timeintervall");
    }
});

socket.on('lightData', function(interval) {
    console.log('user ' + clientID + ' requested data with interval (ms): ' + interval);
    if(interval > 99) {
        timers.push(
            setInterval(function(){
                io.emit('getLightData', 0);
            }, interval)
        );
    } else {
        console.log("too short timeintervall");
    }
});




    socket.on('soilHygrometer', function(data) {
        io.emit('graphSoilHygrometer', data);
        console.log('user ' + clientID + ' gained the data: ' + data);
    });

    socket.on('temperatureSensor', function(data) {
        io.emit('graphTemperatureSensor', data);
        console.log('user ' + clientID + ' gained the data: ' + data);
    });

    socket.on('CO2Sensor', function(data) {
        io.emit('graphCO2Sensor', data);
        console.log('user ' + clientID + ' gained the data: ' + data);
    });

    socket.on('pHSensor', function(data) {
        io.emit('graphPHSensor', data);
        console.log('user ' + clientID + ' gained the data: ' + data);
    });

    socket.on('lightSensor', function(data) {
        io.emit('graphLightSensor', data);
        console.log('user ' + clientID + ' gained the data: ' + data);
    });
 */
