// Graph live data

var waterLevelArray = [];
var airHumidityArray = [];
var soilHumidityArray = [];
var temperatureArray = [];
var luxArray = [];
var CO2Array = [];
var pHArray = [];

var waterPumpPowerArray = [];

var timeArray = [];

var allData = [waterLevelArray, waterPumpPowerArray, 
               soilHumidityArray, luxArray, temperatureArray, 
               CO2Array, pHArray, airHumidityArray];

function printDataValues(){
    var printstr = "";
    for (var i in allData){
        var lastValue = allData[i].length - 1;
        printstr += allData[i][lastValue];
        printstr += "<br>";
    }
    document.getElementById("dataValues").innerHTML = printstr;
}

function updateTime() {
    var hours = new Date().getHours();
    var minutes = new Date().getMinutes();
    var currentTime = hours + ":" + minutes;

    timeArray.push(currentTime);
}
