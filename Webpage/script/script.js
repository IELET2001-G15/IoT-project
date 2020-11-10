// Graph live data

var waterLevelArray = [60, 10, 20, 50]; /* The chart data for "Data 1", altering this array/list changes the graph data */
var waterPumpPowerArray = [10, 15, 20, 35, 40, 70, 100]; /* The chart data for "Data 1", altering this array/list changes the graph data */
var soilHygrometerArray = [2, 100, 50, 130, 20, 10, 50, 420, 50, 10]; /* The chart data for "Data 2", altering this array/list changes the graph data */
var lightSensitivityArray = [60, 10, 20, 50]; /* The chart data for "Data 1", altering this array/list changes the graph data */
var temperatureArray = [60, 10, 20, 50]; /* The chart data for "Data 1", altering this array/list changes the graph data */
var co2Array = [60, 10, 20, 50]; /* The chart data for "Data 1", altering this array/list changes the graph data */
var pHArray = [60, 10, 20, 50]; /* The chart data for "Data 1", altering this array/list changes the graph data */

var timersArray = [];



var allData = [waterLevelArray, waterPumpPowerArray, soilHygrometerArray, lightSensitivityArray, temperatureArray, co2Array, pHArray];
var xaxis = ['01:00', '02:00', '03:00', '04:00', '05:00', '06:00', '07:00', '08:00', '08:00', '09:00', '10:00'];

function printDataValues(){
    var printstr = "";
    for (var i in allData){
        var lastValue = allData[i].length-1;
        printstr += allData[i][lastValue];
        printstr += "<br>";
    }
    document.getElementById("dataValues").innerHTML = printstr;
}


