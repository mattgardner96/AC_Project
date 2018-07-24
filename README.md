# AC_Project

This project is something I've been working on over the last month or so, but I'm just now bringing it to GitHub as I begin to extend the functionality in a big way. I'm trying to control my A/C unit over the internet, by broadcasting an IR signal that mimics the A/C remote's signals, which I scanned in using SEEEDSTUDIO's Grove Infrared Receiver and recv.ino example code.

While my program currently works by typing commands into the Serial Monitor and sending a corresponding IR signal to the A/C, I'm currently working on trying to automate this process using IFTTT's Webhooks service, Dynamic DNS, and an Arduino Yun running as an HTTP server.
