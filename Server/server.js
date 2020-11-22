var fs = require('fs');
var http = require('http');
var express = require('express');
var app = express();

var admin = require("firebase-admin"); //We get the firebase admin library

var serverPort = 2520;

var server = http.createServer(app);
var io = require('socket.io').listen(server);


var username_arr = ["test", "bruker1"];
var password_arr = ["passord", "passord1"];

server.listen(serverPort, function() {
    console.log('listening on *:' + serverPort);
});

//We configure the firebase admin to connect to our database with admin credentials
var serviceAccount = require("./ielet2001-firebase-adminsdk.json"); //This is our admin "password" file (it is a file and it is located in a folder)
var fAdmin = admin.initializeApp({
    credential: admin.credential.cert(serviceAccount),
    databaseURL: "https://ielet2001.firebaseio.com/" //This is the link to our database, this and the password file has to be specified by you
});

//We use the firebase admin credentials to fetch the database object that lets us interact with our database specifically
var db = fAdmin.database();

//A function to generate the current date in a organized format as a string. We want to date our events.
function getDateAsString() {
    var currentDate = new Date(); //We use the JavaScript Date object/function and then configure it
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
    return date; //returns the date string
}

//A function to generate the current time in a organized format as a string. We want to timestamp our events
function getTimeAsString() {
    var currentTime = new Date(); //We use the JavaScript Date object/function and then configure it

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
    return time; //returns the time string
}

//This is our registration key. If the user does not enter, they will not be allowed to register
var regKey = "passord"; //This can be set to whatever you want, and it should probably be more secure than "password"

io.on('connection', function(socket) { //This is the server part of the "what happens when we first connect" function. Everytime a user connects a instance of this is set up for the user privatley
    var regUID = 0; 
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

    client.on('disconnect', function(){ //This function is called for a client when the client is disconnected. The server can then do something even tough the client is disconnected
        clearInterval(timer);
        console.log("user " + clientID + " disconnected, stopping timers if any");

        //When the user disconnects we want to log and store this in the database
        if(regUID != undefined && regUID != "" && regUID != 0) { //This checks that the user is logged in via the regUID

            stopListeningForData(); //Stop client data stream, new database entries will not be sent to the socket client

            var currentDate = getDateAsString(); //Get the date
            var currentTime = getTimeAsString(); //Get the time
            var currentDateTime = currentDate + "-" + currentTime; //Put the date and time together

            db.ref('users/' + regUID).update({ //Here we make a call to our database to update our user in the users directory of the database
                is_active: 0, //We set the sures to not-active
                last_active: currentDateTime, //We timestamp the last time the user was logged on (which is to say when they logged of)
                ip_address: IPArr[3] //We log the last known IP-address of the user, in case this could be usefull
            }).then((snap) => { //The .then function is called after a sucessfull call/request to the databas
                console.log("User " + regUID + " updated last_active to " + currentDateTime + " and ip_address to " + IPArr[3]);
                console.log("User " + regUID + " is no longer ative"); //We console log some properties about the user
            });
        }
    });
  
     //Socket authentication function
     socket.on('auth', function(username, password) {

        if(username != undefined && password != undefined) { //Check to see that the entered username and password isnt empty

            var local_auth = false; //This will be used to keep track if a username/password matches or not in the next lines of code

            for (var i = 0; i < username_arr.length; i++) { //We for loop trough the arrays of username and password declared earlier

                if(username == username_arr[i]) { //If the username sent from the webpage matches or not
                    if (password == password_arr[i]){ //If the password matches should the username alsp match
                        console.log("Username and password matches");
                        auth = true; //We set the global auth to true, this now means that this current client/user can call any other functions
                        client.emit('authState', 1); //Tells the client that it has been authenticated
                        local_auth = true; //The local auth is set to true to ensure that the later if-statement does not kick in
                        return;
                    } else {
                        console.log("Username matches, but password does not");
                        local_auth = false; //If the username matches, but the password does not
                    }
                } else {
                    console.log("Username does not match");
                    local_auth = false; //If the username does not match
                }
            }

            if(local_auth != true) { //After the loop is done (and not broken by the return command) run this to set global auth to false
                auth = false; //No username or password matches, so the user is not authenticated
                client.emit('authState', 0); //Tells the client that is has not been authenticated
                console.log("No usernames and password matches.");
            }

        } else {
            console.log("Username og password not entered"); //If nothing is sent to the server with the auth request
        }
    });

     //Register a user with the register page and functions
     socket.on('regUser', function(key, username, password) { //One needs to call regUser with a key, username and password

        if(key == regKey) { //This checks the regkey to see if the user has the right key (that they are allowed to register a user)
            console.log("-------User Registration-------"); //Log this function clearly
            console.log("Username:" + username);
            console.log("Password:" + password);

            var regDate = getDateAsString(); //Get the register date
            var currentTime = getTimeAsString(); //Get the register time
            console.log(regDate);
            console.log(currentTime);

            db.ref('users/').push( { //Create a new user in the user database
                username: username,
                password: password,
                register_date: regDate,
                is_active: 0,
                last_active: regDate + "-" + currentTime,
                ip_address: IPArr[3]
            }).then((snap) => { //After the user is successfully registered do something
                console.log("Data was sent to server and user " + username + " was registered");
                client.emit("regSuccess", username); //We tell the client that the user was successfully registered
                console.log("------------- End User Reg. -------------");
            });

        } else {
            console.log("Registerkey does not match, you are not allowed to register a user");
            client.emit("regDenied"); //If the user does not enter the correct key to register tell them
        }
    });

    //Authenticate a user on the controlpanel/client
    socket.on('authUser', function (username, password) { //One needs to call authUser with a username and password
        var data = db.ref('users').orderByChild('username').equalTo(username); //This is a firebase db query
        //We want to search the "users" database, and in that database we want to target on of the subfields called username
        //Then with that target we go trough all the user entries and look for the user with a username equal to our entered username
        //When that user/object is found we send it from the database to Node.js to do something with the data (which happens below)

        data.once('value', function(snap) { //We call this function one time, when the data is fetched from the database. The data is inside the snap variable

        }).then((snap) => { //When the data is successfully fetched (finished downloading properly) we do something with ti

            if(snap.numChildren() == 1) { //We check to see that there is only one user with the username
                //We have no registration check to see if there already exists a user with a given username
                console.log("User exists");
                var value = snap.val(); //This is where we get the JSON-array of all returned data entries from the database (even tough it is only 1 object returned)
                console.log(value);

                var UID = Object.keys(value)[0]; //Here we use a function that retrieves all the data keys (the ID of the data entry)

                var output = value[UID]; //Then we use the ID to retrieve the data from the JSON-array
                console.log(output);

                if(output.password == password) { //Then, if the password matches we proceed. The password can not be checked before we have fetched the data
                    console.log("Username and password matches, user is authenticated");
                    regUID = UID; //Does it match we set the client-global variable regUID to the UID of the data entry. We call this ID the UserID (UID)
                    console.log(regUID);
                    client.emit("authSuccess", username); //Now we tell the client that it has successfully authenticated with the server

                    startListeningForData(); //Start client data stream, everytime the database gets a new entry the socket client will be sent the data

                    if(regUID != undefined && regUID != "" && regUID != 0) { //We use the same check as in disconnected to see if the user is registered on the Node/Socket server
                        var currentDate = getDateAsString(); //Get date
                        var currentTime = getTimeAsString(); //Get time
                        var currentDateTime = currentDate + "-" + currentTime; //Fusion them together

                        db.ref('users/' + regUID).update({ //Tell the database that the user has logged in and is now active
                            is_active: 1, //The user is active
                            last_active: currentDateTime, //Save the last_active time of a user to now
                            ip_address: IPArr[3] //Save the IP-address of the user
                        }).then((snap) => { //When the user update has been saved
                            console.log("User " + regUID + " updated last_active to " + currentDateTime + " and ip_address to " + IPArr[3]);
                        });
                    }

                } else {
                    console.log("Userpassword does not match.");
                    client.emit("authFail"); //Tell the client that the authentication has failed
                }

            } else {
                console.log("Error finding users, either there are to many or none.");
                client.emit("authFail"); //Tell the client that the authentication has failed
            }

        })

    });

    /**
     * Handles event when a web page wants to change light brightness on ESP32.
     * Forwards power to ESP32
     * @param power the power which is set to the light in bits
     */
    socket.on('lightPower', function(power) {
        if(regUID != undefined && regUID != "" && regUID != 0) { //Check if the user is authenticated
            io.emit('changeLightPower', power); //This is the actual socket.io emit function to ESP
            console.log('user ' + clientID + ' changed the light power to [%]: ' + power);
        } else {
            console.log("User is not authenticated");
        }
    });

    /**
     * Handles event when a web page wants to change water pump power on ESP32. 
     * Forwards power to ESP32
     * @param power the power which is set to the water pump in bits
     */
    socket.on('waterPumpPower', function(power) {
        if(regUID != undefined && regUID != "" && regUID != 0) { //Check if the user is authenticated
            io.emit('changeWaterPumpPower', power);
            console.log('user ' + clientID + ' changed the water pump power to [%]: ' + power);
        } else {
            console.log("User is not authenticated");
        }
    });

    /**
     * Handles event when a web page wants to change vent opening on ESP32.
     * Forwards angle to ESP32
     * @param angle the angle which the servo motor turns in degrees
     */
    socket.on('ventAngle', function(angle) {
        if(regUID != undefined && regUID != "" && regUID != 0) { //Check if the user is authenticated
            io.emit('changeVentAngle', angle);
            console.log('user ' + clientID + ' changed the vent angle to [deg]: ' + angle);
        } else {
            console.log("User is not authenticated");
        }
        
    });

    /**
     * Handles event when web page requests sensor data from ESP32.
     * Clears previous timers to avoid more than one request interval at once and 
     * forwards the request to ESP32
     * @param request the sensor that is requested. "all" if all sensors
     * @param interval the time between each consecutive request
     */

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

    /**
     * Handles event when web page does not want data anymore. Stops data requests 
     * by clearing timer
     */

    socket.on('stopDataFromBoard', function() { //This function stops all the timers set by a user so that data will no longer be sent to the webpage
        if(regUID != undefined && regUID != "" && regUID != 0) { //Check if the user is authenticated
            clearInterval(timer);
            console.log('user ' + clientID + ' cleared data request interval');
        } else {
            console.log("User is not authenticated");
        }
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
