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


var title = document.getElementById("title-text");
var submitBtn = document.getElementById("submit-btn");

var nameInp = document.getElementById("name-inp");
var passwordInp = document.getElementById("password-inp");
var keyInp = document.getElementById("key-inp");


function authUser(username, password) { //The auth function

    //Emmiting "auth"-does NOT use the database
    //Emmiting "authUser" USES the database
    //We have to add users in the database using the register page fist

    socket.emit('authUser', username, password); //We emit the username and password to the server. This then checks the credentials in the local database

    socket.once('authState', function (state) { //Response from the server

        if (state == 0) { //If the server tells us 0, we are not authenticated because the username/password did not match
            alert("Du tastet inn feil brukernavn eller passord."); //We are alerted with a message
            console.log("Client is not authenticated");
            location.reload(); //This reload the webpage so the user can not do anything if they dont manage to login
        } else if (state == 1) { //IF the server tells us 1, we are authenticated and can proceed to use the webpage
            alert("Du er logget in."); //We are alerted with a message
            console.log("Client is authenticated");
        }
    });
}


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

function updateTime() {
    var hours = new Date().getHours();
    var minutes = new Date().getMinutes();
    var currentTime = hours + ":" + minutes;
    timeArray.push(currentTime);
}

function avoidArrayOverflow() { //We must remember to include timeArray in this function
    const limit = 30;

    for (var i in allData){     //Suggestion: Include timeArray in allData, and display time in live data table
        if (allData[i].length >= limit){
            allData[i].shift();
        }
    }

    if (timeArray.length >= limit){
        timeArray.shift();
    }
}

function ventStatus(){
    var ventAngleDeg = 30;
    var ventStatus = document.getElementById("ventStatusValue").checked;

    if(ventStatus == true){
        ventAngle(ventAngleDeg);
    } else {
        ventAngle(-ventAngleDeg);
    }
}

function humidityConverter(data){
    var newData = data*100/4095;
    newData = Math.round(newData);
    return newData
}

function waterPumpPowerPush(){
    var powerValue = document.getElementById("myRange").value;
    waterPumpPowerArray.push(powerValue);
}

function waterPumpPowerConverter(data){
    var newData = data*255/100;
    newData = Math.round(newData);
    return newData;
}

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
