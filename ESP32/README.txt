The code for the ESP32 requires some libraries to be able to successfully compile to the microcontroller. 
Simply move the libraries in the "library" folder into your own local library directory.
You have to edit the WiFi-credentials in main.ino such that the microcontroller is able to get access to the internet.
Then compile the sketch called main.ino on to your ESP32.
