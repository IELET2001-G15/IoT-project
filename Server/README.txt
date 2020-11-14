This folder contains the server.

The Raspberry Pi needs Node.js to run JavaScript-files. Use the command "sudo apt-get install nodejs" in the Raspberry Pi's 
terminal to install Node.js. Make a directory that will contain the server-file. For example "/server". Upload server.js to 
this location. The server has some dependencies. Install NPM by using the command "sudo apt-get install npm". With NPM you 
can install the necessary packages by using the commands "npm i file-system", "npm i express" and "npm i socket.io" while 
residing in the "/server"-directory. To start the server use the command "node server.js" while in the same directory.