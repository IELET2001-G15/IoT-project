// Constants and variables
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

var title = document.getElementById("title-text");
var divMain = document.getElementById("main-content-div");
var divReg = document.getElementById("main-register-div");
var submitBtn = document.getElementById("submit-btn");
var loginBtn = document.getElementById("login-btn");
var nameInp = document.getElementById("name-inp");
var passwordInp = document.getElementById("password-inp");
var keyInp = document.getElementById("key-inp");

// When loading the website, hide the second main window, control panel
window.onload = divMain.style.display = "none";

// When login in successfully, run switchPage function and switch page to second main window
loginBtn.onclick = function() {
    switchPage();
}

submitBtn.onclick = function() {
    console.log("Register button clicked.");
    console.log(keyInp.value);
    console.log(nameInp.value);
    console.log(passwordInp.value);
    registerUser(keyInp.value, nameInp.value, passwordInp.value);
    switchPage();
};

// Function list

// Switch page when successfully authorised
function switchPage (){
    divReg.style.display = "none";
    divMain.style.display = "block";

    var username_input = prompt("Skriv inn brukernavnet ditt ", "1");
    var password_input = prompt("Skriv in passordet ditt", "1");

    if (username_input != undefined && password_input != undefined) {
        authUser(username_input, password_input);
    }
    controlPanel();
}

// Authorise user
function authUser(username, password) {
    socket.emit('authUser', username, password);
    socket.once('authState', function (state) {

        if (state == 0) {
            alert("Du tastet inn feil brukernavn eller passord.");
            console.log("Client is not authenticated");
            location.reload();
        } else if (state == 1) {
            alert("Du er logget in.");
            console.log("Client is authenticated");
        }
    });
}

// Printing data values in div 4
function printDataValues(){
    var printstr = "";
    for (var i in allData) {
        var lastValue = allData[i].length - 1;
        if (allData[i][0] !== undefined){
            printstr += allData[i][lastValue];
        } else {
            printstr += "N/A";
        }
        printstr += "<br>";
    }
    document.getElementById("dataValues").innerHTML = printstr;
}

// Retrieves current time through js, which makes the x-axis of the graph plot
function updateTime() {
    var hours = new Date().getHours();
    var minutes = new Date().getMinutes();
    var currentTime = hours + ":" + minutes;
    timeArray.push(currentTime);
}

// Avoiding overflow when the arrays contain too much information
// A solution to this could be to save the data in a database before deleting it,
// thus saving the measured units
function avoidArrayOverflow() {
    const limit = 30;
    for (var i in allData){
        if (allData[i].length >= limit){
            allData[i].shift();
        }
    }

    if (timeArray.length >= limit){
        timeArray.shift();
    }
}

// Open and closes the vent
function ventStatus(){
    var ventAngleDeg = 30;
    var ventStatus = document.getElementById("ventStatusValue").checked;

    if(ventStatus == true){
        ventAngle(ventAngleDeg);
    } else {
        ventAngle(-ventAngleDeg);
    }
}

// Mapping the humidity data
function humidityConverter(data){
    var newData = data*100/4095;
    newData = Math.round(newData);
    return newData
}

// Pushing the current water pump power value in an array to display it
function waterPumpPowerPush(){
    var powerValue = document.getElementById("myRange").value;
    waterPumpPowerArray.push(powerValue);
}

// Mapping the water pump power before sending it through socket
function waterPumpPowerConverter(data){
    var newData = data*255/100;
    newData = Math.round(newData);
    return newData;
}

// Display control panel on or off
function controlPanel() {
    var divtitle = document.getElementById("div2-title");
    var divU1 = document.getElementById("div2-btn-box-u1");
    var divU2 = document.getElementById("div2-btn-box-u2");
    var divD1 = document.getElementById("div2-btn-box-d1");

    if (divU1.style.display === "none") {
        divU1.style.display = "block";
        divU2.style.display = "block";
        divD1.style.display = "block";
        divtitle.innerHTML = "MANUAL CONTROL PANEL ON"

    } else {
        divU1.style.display = "none";
        divU2.style.display = "none";
        divD1.style.display = "none";
        divtitle.innerHTML = "MANUAL CONTROL PANEL OFF"
    }
}

// Automation mode, not used but can be used to automatically water the plants
// when a soil humidity value is under a certain value. Then activate water pump power for a spesific
// time.
function autoMode() {
    const waterDryLevel = 200;
    const waterLevelPump = 100;
    var lastValue = soilHumidityArray.length - 1;
    if (soilHumidityArray[lastValue] < waterDryLevel){
        waterPumpPower(waterLevelPump);
    } else {
        waterPumpPower(0);
    }
}