var socket = io.connect('192.168.137.145:2520', {secure: false});

var interval = 1000;
var timer;

socket.on('clientConnected', function(id, ip) {
    console.log('Client recevied ID: ' + id);
    console.log("Client IP: " + ip);
});

//################################################################################################

socket.on('pushWaterLevel', function(data) {
    console.log('Water level data was received: ' + data);
    waterLevelArray.push(Number(data));
});

socket.on('pushSoilHumidity', function(data) {
    console.log('Soil humidity data was received: ' + data);
    soilHumidityArray.push(Number(data));
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

//###################################################################################

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

//#####################################################################################################

function requestDataFromBoard(request) {
    socket.emit('requestDataFromBoard', request);
    console.log('requestDataFropushd was called with request: ' + request);
}

function stopDataFromBoard() {
    clearTimeout(timer);
    socket.emit('stopDataFromBoard');
    console.log("stopDataFromBoard was called");
}

function changeInterval(newInterval) {
    timer = setInterval(function() {
                updateTime();
                printDataValues();
                myLineChart.update();
            }, newInterval);
    interval = newInterval;
    socket.emit('changeInterval', newInterval);
    console.log('changeInterval was called with newInterval [ms]: ' + newInterval);
}
