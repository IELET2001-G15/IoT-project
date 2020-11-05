// Your web app's Firebase configuration
var firebaseConfig = {
    apiKey: "AIzaSyAxPdkVcE4IbbA6ZF1zi4uZjPcyJNHD5jk",
    authDomain: "ielet2001.firebaseapp.com",
    databaseURL: "https://ielet2001.firebaseio.com",
    projectId: "ielet2001",
    storageBucket: "ielet2001.appspot.com",
    messagingSenderId: "209139214302",
    appId: "1:209139214302:web:b50426ecce3cc060fa5f4f",
    measurementId: "G-FZY3N43HLZ"
};

// Initialize Firebase
firebase.initializeApp(firebaseConfig);
firebase.analytics();

// Get elements
const plantHumidity = document.getElementById('humidity'); //Needs to be fetched from index.html

// Create references
const dbRefHumidity = firebase.database().ref().child(humidity); //Sends the value to the root of the database, and appends the value.

// Sync object changes
dbRefHumidity.on("value", snap => console.log(snap.val())); //Syncronises data in real time. Sends changes to the web-console.