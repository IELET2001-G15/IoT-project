var socket = io.connect('192.168.137.151:2520', {secure: false});
var timer;

socket.on('clientConnected', function(id, ip) {
    console.log('Client recevied ID: ' + id);
    console.log("Client IP: " + ip);
});

// All communication from ESP to website
socket.on('pushWaterLevel', function(data) {
    console.log('Water level data was received: ' + data);
    waterLevelArray.push(Number(data));
});

socket.on('pushSoilHumidity', function(data) {
    console.log('Soil humidity data was received: ' + data);
    var newData = humidityConverter(data);
    soilHumidityArray.push(Number(newData));
});

socket.on('pushAirHumidity', function(data) {
    console.log('Air humidity data was received: ' + data);
    airHumidityArray.push(Number(data));
});

socket.on('pushTemperature', function(data) {
    console.log('Temperature data was received: ' + data);
    temperatureArray.push(Number(data));
});

socket.on('pushCO2', function(data) {
    console.log('CO2 data was received: ' + data);
    CO2Array.push(Number(data));
});

socket.on('pushpH', function(data) {
    console.log('pH data was received: ' + data);
    pHArray.push(Number(data));
});

socket.on('pushLux', function(data) {
    console.log('Lux data was received: ' + data);
    luxArray.push(Number(data));
});

// All communication from website to ESP
function lightPower(power) {
    socket.emit('lightPower', power);
    console.log('lightPower was called with power [bits]: ' + power);
}

function waterPumpPower(power) {
    var newPower = waterPumpPowerConverter(power); //convert from 0-100 to 0-255
    socket.emit('waterPumpPower', newPower);
    console.log('waterPumpPower was called with power [bits]: ' + newPower);
}

function ventAngle(angle) {
    socket.emit('ventAngle', angle);
    console.log('ventAngle was called with angle [deg]: ' + angle);
}

// Register a user
function registerUser(key, username, password) {
    console.log(key);
    console.log(username);
    console.log(password);
    socket.emit('regUser', key, username, password);

    socket.once('regSuccess', function (username) {
        console.log('Brukeren din ble registrert!');
        title.innerHTML = `Brukeren din ${username} ble registrert!`;
    });

    socket.once('regDenied', function () {
        title.innerHTML = `Brukeren din ${username} ble ikke registrert.`;
        console.log('Registrering feilet');
    });
}

// Main functionality of the website
function requestDataFromBoard(request, interval) {
    clearInterval(timer);
    timer = setInterval(function() {
        waterPumpPowerPush();
        avoidArrayOverflow();
        myLineChart.update();
        printDataValues();
        updateTime();
        // automode() can be used to regulate the water plant automatically, but we have no pump atm
    }, interval);
    socket.emit('requestDataFromBoard', request, interval);
    console.log('requestDataFromBoard was called with request/interval [ms]: ' + request + '/' + interval);
}

// Stops the communication between esp and server
function stopDataFromBoard() {
    clearInterval(timer);
    socket.emit('stopDataFromBoard');
    console.log("stopDataFromBoard was called");
}
