var socket = io.connect('192.168.137.145:2520', {secure: false}); //This line declares a socket.io object to var "socket" and connects to the server (change the IP-address and port to your own)
//The "secure: false" tells if the connection will be encrypted or not. Since we will not encrypt our connections, this is false.

//Socket.io has several functions. The .on function refers to what will happen when the client receive a call called 'connect' from the server
//View it as calling a function remotley. The server tells the client it wants to call this function with no arguments.
socket.on('connect',function() { //When you connect to the server (and it works) call this function
    console.log('Client has connected to the server!'); //The client prints this message
}); //The 'connect' function/identifier is the standard procedure. To make something more we have to make it ourselves

socket.on('clientConnected', function(id, ip) { //This is our selfmade functions. Here we can have the server return arguments (data) that we need
    console.log('Client recevied ID: ' + id); //In this case the server will tell us what our local ID is (auto assigned)
    console.log("Client IP: " + ip);//And it will tell us what our IP-address
});

socket.on('graphWaterLevelSensor', function(data) { //Received data from the server who is forwarding it to us from the ESP32
    console.log('WaterLevelSensor data was received: ' + data);
    console.log(Number(data));
    waterLevelArray.push(Number(data)); //This pushes data to the array that stores all the chart data
    myLineChart.update(); //This updates the chart
});

socket.on('graphTimers', function(timers) { //Received data from the server who is forwarding it to us from the ESP32
    console.log('Timer data was received: ' + timers);
    console.log(Number(timers));
    timersArray = timers;
    myLineChart.update(); //This updates the chart
});


//In this function (which is essentially built up the same as a void function in Arduino) we want to send something to the server
//For this we use the other important Socket.io function, .emit(arg). Here we are telling our socket object so call the "changeLEDState" function
//on the server with the "state" argument. By calling the function on the server we mean that we send data to the server that tells it to do something
function changeLEDState(state) {
    //This function controls wether a LED-light is on or of
    socket.emit('changeLEDState', state); //Here the actual socket-object function is called. If we want a response we will have to set up a function (.on) like earlier.
    console.log("changeLEDState called");
}

function lightPower(power) {
    socket.emit('lightPower', power);
    console.log("lightPower called");
}

function waterPumpPower(power) {
    socket.emit('waterPumpPower', power);
    console.log("waterPumpPower called");
}

//This function also emits something to the server. But in this case we want something a little bit more complex to happen.
//Since Arduino has a limited amount of timers, and using millis can be annoying, we have the possibilties of handing that task over to JavaScript on Node.js
//The function we are calling here will tell the server to set up a JavaScript timer, which then will periodically send a message to the ESP32 asking for data.
//Since the ESP32 easily can react to such a request it sends the data with no problems, and with no timers in use.
//This means we dont have to use the delay() function or the millis() function in Arduino, we can just let Node and JavaScript fix the tracking of time for us
//This is the function that will make the ESP32 transmit data to the server, and not the other way around
function waterLevelData(interval) {
    socket.emit('waterLevelData', interval); //Here we tell the server to call the function "requestDataFromBoard" with a argument called "intervall"
    //The intervall value is the period of time between each data transmit from the ESP32 to the server. Typical values can be everything form 100ms to 100s
    console.log("waterLevelData was called with interval: " + interval);
} //Be careful to not set the interval value to low, you do not want to overflood your server with data/requests

function requestDataFromBoard(interval){
    waterLevelData(interval);
    printDataValues();

    //soilHygrometerData(interval);
    //temperatureData(interval);
    //lightData(interval);
    //co2Data(interval);
    //pHData(interval);
}

function stopDataFromBoard() { //Tells the server to stop all timers so that data is no longer sent from the ESP32 to the webpage
    socket.emit('stopDataFromBoard'); //Here we tell the server to call the function "stopDataFromBoard"
    console.log("stopDataFromBoard was called");
}






/*

socket.on('graphSoilHygrometer', function(data) { //Received data from the server who is forwarding it to us from the ESP32
    console.log('SoilHygrometer data was received: ' + data);
    console.log(Number(data));
    soilHygrometerArray.push(data); //This pushes data to the array that stores all the chart data
    myLineChart.update(); //This updates the chart
});
*/






/*

function soilHygrometerData(interval) {
    socket.emit('soilHygrometerData', interval);
    console.log("soilHygrometerData was called with interval: " + interval);
}

function temperatureData(interval){
    socket.emit('temperatureData', interval);
    console.log("temperatureData was called with interval: " + interval);
}

function co2Data(interval){
    socket.emit('co2Data', interval);
    console.log("co2Data was called with interval: " + interval);
}

function pHData(interval){
    socket.emit('pHData', interval);
    console.log("pHData was called with interval: " + interval);
}

function lightData(interval){
    socket.emit('lightData', interval);
    console.log("lightData was called with interval: " + interval);
}


 */