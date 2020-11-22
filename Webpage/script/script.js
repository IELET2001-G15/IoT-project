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



var username = prompt("Skriv inn brukernavnet ditt ", "1"); //This asks you for a username when the webpage first loads
var password = prompt("Skriv in passordet ditt", "1"); //This asks you for a password when the webpage first loads

if (username != undefined && password != undefined) { //If the username and password is actually entered, empty input will not send the auth request
    authUser(username, password); //Call the auth function on the client
}

function authUser(username, password) { //The auth function

    socket.emit('auth', username, password); //We emit the username and password to the server. This then checks the credentials in the local database

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

function printDataValues() {
    var printstr = "";
    for (var i in allData) {
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


var overflow = 1000;

function avoidArrayOverflow() { //We must remember to include timeArray in this function
    for (var i in allData) {     //Suggestion: Include timeArray in allData, and display time in live data table
        if (allData[i].length <= overflow) {
            allData[i].shift();
        }
    }
}