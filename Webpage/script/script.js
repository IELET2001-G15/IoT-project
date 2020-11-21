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
               CO2Array, pHArray, airHumidityArray, timeArray];

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

function avoidArrayOverflow() { //We must remember to include timeArray in this function
    for (var i in allData){     //Suggestion: Include timeArray in allData, and display time in live data table
        if (allData[i].length >= 100){
            allData[i].shift();
        }
    }
}