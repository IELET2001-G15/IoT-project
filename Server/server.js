
// Constants and variables
var fs = require('fs');
var http = require('http');
var express = require('express');
var app = express();

// We get the firebase admin library
var admin = require("firebase-admin");

var serverPort = 2520;
var server = http.createServer(app);
var io = require('socket.io').listen(server);

// Server starts listening to a port
server.listen(serverPort, function() {
    console.log('listening on *:' + serverPort);
});

// We configure the firebase admin to connect to our database with admin credentials
var serviceAccount = require("./ielet2001-firebase-adminsdk.json");
var fAdmin = admin.initializeApp({
    credential: admin.credential.cert(serviceAccount),
    databaseURL: "https://ielet2001.firebaseio.com/"
});

// We use the firebase admin credentials to fetch the database object that lets us interact with our database specifically
var db = fAdmin.database();

// A function to generate the current date in a organized format as a string. We want to date our events.
function getDateAsString() {
    var currentDate = new Date(); // We use the JavaScript Date object/function and then configure it
    var currentMonth;
    var currentDay;

    if(currentDate.getMonth() < 10) {
        currentMonth = "0" + String(currentDate.getMonth());
    } else {
        currentMonth = String(currentDate.getMonth());
    }

    if(currentDate.getDay() < 10) {
        currentDay = "0" + String(currentDate.getDay());
    } else {
        currentDay = String(currentDate.getDay());
    }

    var date = currentDate.getFullYear() + "-" + currentMonth + "-" + currentDay;
    return date;
}

// A function to generate the current time in a organized format as a string. We want to timestamp our events
function getTimeAsString() {
    var currentTime = new Date();

    var currentHour;
    var currentMinute;
    var currentSecond;

    if(currentTime.getHours() < 10) {
        currentHour = "0" + String(currentTime.getHour());
    } else {
        currentHour = String(currentTime.getHours());
    }

    if(currentTime.getMinutes() < 10) {
        currentMinute = "0" + String(currentTime.getMinutes());
    } else {
        currentMinute = String(currentTime.getMinutes());
    }

    if(currentTime.getSeconds() < 10) {
        currentSecond = "0" + String(currentTime.getSeconds());
    } else {
        currentSecond = String(currentTime.getSeconds());
    }

    var time = currentHour + "-" + currentMinute + "-" + currentSecond;
    return time;
}

// This is our registration key. If the user does not enter, they will not be allowed to register
var regKey = "MidjoSkyen";

// This is the server part of the "what happens when we first connect" function.
// Everytime a user connects a instance of this is set up for the user privatkey.
io.on('connection', function(socket) {
    console.log('a user connected');
    var regUID = 0;
    var clientID = socket.id;
    var client = io.sockets.connected[clientID];
    var clientIPRAW = client.request.connection.remoteAddress;
    var IPArr = clientIPRAW.split(":",4);


    console.log("User ID: " + clientID);
    console.log("User IP: " + IPArr[3]);

    io.emit("clientConnected", clientID, IPArr[3]);

    var timer;

    // Handles event when client disconnects. Stops data requests by clearing timer
    // This function is called for a client when the client is disconnected.
    // The server can then do something even tough the client is disconnected
    // when the user disconnects we want to log and store this in the database
    client.on('disconnect', function(){
        if(regUID != undefined && regUID != "" && regUID != 0) {
            clearInterval(timer);
            console.log("user " + clientID + " disconnected, stopping timers if any");
            stopListeningForData();

            var currentDate = getDateAsString();
            var currentTime = getTimeAsString();
            var currentDateTime = currentDate + "-" + currentTime;

            // Here we make a call to our database to update our user in the users directory of the database
            db.ref('users/' + regUID).update({
                is_active: 0,
                last_active: currentDateTime,
                ip_address: IPArr[3]
            }).then((snap) => { // The .then function is called after a sucessfull call/request to the database
                console.log("User " + regUID + " updated last_active to " + currentDateTime + " and ip_address to " + IPArr[3]);
                console.log("User " + regUID + " is no longer ative");
            });
        }
    });
    
    socket.on('regUser', function(key, username, password) { // One needs to call regUser with a key, username and password

        // This checks the regkey to see if the user has the right key (that they are allowed to register a user)
        if(key == regKey) {
            console.log("-------User Registration-------");
            console.log("Username:" + username);
            console.log("Password:" + password);

            var regDate = getDateAsString();
            var currentTime = getTimeAsString();
            console.log(regDate);
            console.log(currentTime);

            // Create a new user in the user database
            db.ref('users/').push( {
                username: username,
                password: password,
                register_date: regDate,
                is_active: 0,
                last_active: regDate + "-" + currentTime,
                ip_address: IPArr[3]
            }).then((snap) => { //After the user is successfully registered do something
                console.log("Data was sent to server and user " + username + " was registered");
                client.emit("regSuccess", username);
                console.log("------------- End User Reg. -------------");
            });

        } else {
            // If the user does not enter the correct key to register tell them
            console.log("Registerkey does not match, you are not allowed to register a user");
            client.emit("regDenied");
        }
    });

    // Authenticate a user on the controlpanel/client
    // USING THE DATABASE!
    socket.on('authUser', function (username, password) {
        var data = db.ref('users').orderByChild('username').equalTo(username);

        // We want to search the "users" database, and in that database we want to target on of the subfields called username
        // Then with that target we go trough all the user entries and look for the user with a username equal to our entered username
        // When that user/object is found we send it from the database to Node.js to do something with the data (which happens below)
        data.once('value', function(snap) {

        }).then((snap) => {
            if(snap.numChildren() == 1) {
                console.log("User exists");
                var value = snap.val();
                console.log(value);

                var UID = Object.keys(value)[0];
                var output = value[UID];
                console.log(output);

                // If the password is correct, authentication has succeeded
                if(output.password == password) {
                    console.log("Username and password matches, user is authenticated");
                    regUID = UID;
                    console.log(regUID);
                    client.emit("authSuccess", username);
                    client.emit("authState", 1);
                    startListeningForData();

                    // We use the same check as in disconnected to see if the user is registered on the Node/Socket server
                    if(regUID != undefined && regUID != "" && regUID != 0) {
                        var currentDate = getDateAsString(); //Get date
                        var currentTime = getTimeAsString(); //Get time
                        var currentDateTime = currentDate + "-" + currentTime;

                        // Tell the database that the user has logged in and is now active
                        db.ref('users/' + regUID).update({
                            is_active: 1,
                            last_active: currentDateTime,
                            ip_address: IPArr[3]
                        }).then((snap) => {
                            console.log("User " + regUID + " updated last_active to " + currentDateTime + " and ip_address to " + IPArr[3]);
                        });
                    }

                } else {
                    // Tell the client that the authentication has failed
                    console.log("Userpassword does not match.");
                    client.emit("authFail");
                    client.emit("authState", 0);
                }

            } else {
                // Tell the client that the authentication has failed
                console.log("Error finding users, either there are to many or none.");
                client.emit("authFail");
                client.emit("authState", 0);
            }

        })

    });

    // Handles event when a web page wants to change light brightness on ESP32.
    // Forwards power to ESP32
    // @param power the power which is set to the light in bits
    socket.on('lightPower', function(power) {
        if(regUID != undefined && regUID != "" && regUID != 0) { //Check if the user is authenticated
            io.emit('changeLightPower', power); //This is the actual socket.io emit function to ESP
            console.log('user ' + clientID + ' changed the light power to [%]: ' + power);
        } else {
            console.log("User is not authenticated");
        }
    });

    // Handles event when a web page wants to change water pump power on ESP32.
    // Forwards power to ESP32
    // @param power the power which is set to the water pump in bits
    socket.on('waterPumpPower', function(power) {
        if(regUID != undefined && regUID != "" && regUID != 0) { //Check if the user is authenticated
            io.emit('changeWaterPumpPower', power);
            console.log('user ' + clientID + ' changed the water pump power to [%]: ' + power);
        } else {
            console.log("User is not authenticated");
        }
    });

    // Handles event when a web page wants to change vent opening on ESP32.
    // Forwards angle to ESP32
    // @param angle the angle which the servo motor turns in degrees
    socket.on('ventAngle', function(angle) {
        if(regUID != undefined && regUID != "" && regUID != 0) { //Check if the user is authenticated
            io.emit('changeVentAngle', angle);
            console.log('user ' + clientID + ' changed the vent angle to [deg]: ' + angle);
        } else {
            console.log("User is not authenticated");
        }
        
    });


     // Handles event when web page requests sensor data from ESP32.
     // Clears previous timers to avoid more than one request interval at once and
     // forwards the request to ESP32
     // @param request the sensor that is requested. "all" if all sensors
     // @param interval the time between each consecutive request
    socket.on('requestDataFromBoard', function(request, interval) { // Receives from webpage
        if(regUID != undefined && regUID != "" && regUID != 0) { //Check if the user is authenticated
            clearInterval(timer);
            timer = setInterval(function() {
                io.emit('sendData', request);
            }, interval);
            console.log('user ' + clientID + ' requested ' + request + ' data with interval [ms]: ' + interval);

        } else {
            console.log("User is not authenticated");
        }
    });

    // Handles event when web page does not want data anymore. Stops data requests
    // by clearing timer
    socket.on('stopDataFromBoard', function() { //This function stops all the timers set by a user so that data will no longer be sent to the webpage
        if(regUID != undefined && regUID != "" && regUID != 0) { //Check if the user is authenticated
            clearInterval(timer);
            console.log('user ' + clientID + ' cleared data request interval');
        } else {
            console.log("User is not authenticated");
        }
    });

    // Handles a series of events when ESP32 sends its sensor data.
    // Forwards the data to the web page
    // @param data the data sent from the ESP32
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


    //One can also write normal functions inside the io.on connection
    //This function sends new database data to the socket client (normally webpage) automatically
    function sendDataToClient(snap) {
        var value = snap.val(); //Data object from Firebase

        var DataID = Object.keys(value)[0]; //Here we use a function that retrieves all the data keys (the ID of the data entry)
        var data = value[DataID]; //Then we use the ID to retrieve the data from the JSON-array
        console.log("Data: " + data);
        client.emit('data', data); //We emit to the same listener on the webpage as in the earlier io.emit command ('data') in the dataFromBoard function
    }

    //This function starts the stream of data, everytime the dataFromBoard socket function saves data to the database it is detected here
    function startListeningForData() {
        db.ref('sensordata/' /* + regUID*/).limitToLast(1).on('child_added', sendDataToClient); //Sets up a detection for new data
    }

    //Stop the datastream from the database to the socket client (normally webpage)
    function stopListeningForData() {
        db.ref('sensordata/' /* + regUID*/).off('child_added', sendDataToClient); //Stops detection for new data
    }
});
