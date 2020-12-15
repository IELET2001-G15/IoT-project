This folder contains the web page.

To establish a connection between the socket and the server, make sure that the right ip is addressed.
Go to .../Webpage/script/socket.js, where the first line of code inside the file provides the ip destination.

NB! When making any changes to the code, do not edit any content in the libraries folder. They are mandatory
for the script to work.

The Raspberry Pi needs NGINX to handle HTTP-requests. Use the command "sudo apt-get install nginx" in the Raspberry Pi's 
terminal to install NGINX. Check that the process is running with the command "sudo systemctl status nginx". There should 
now be a directory with the address "/var/www/html". This is the HTTP-servers directory. Replace the index.html file in 
this directory with all the files and folders in the directory called "Webpage" in the source code. 

NB! Do not put the "Webpage"-folder in this directory. I.e. index.html should have the path "/var/www/html/index.html".

