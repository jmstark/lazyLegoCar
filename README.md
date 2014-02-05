lazyLegoCar
===========

Lego car with basic collision avoidance/ path planning


Setup:
===========

Starting the car:
===========

Prerequisite:
Install the App "HelloWorld" (should be renamed in the future) on your smartphone: The compiled app is located in the github repository. The relative path is HelloWorld/bin/HelloWorld.apk.
On your smartphone, the app is named "LegoCarControl".

If everything is compiled and in place (as is the case after the final presentation), to start the car you have to:
1. Turn on the battery.
2. Connect the ethernet port of the raspberry to your pc. Otherwise, it will not start. But you do not need to send anything over the ethernet connection.
3. The raspberry needs power over Micro-USB (a normal charging cable for a smartphone is sufficient). Plug it into your PC.
4. After about 30 seconds, the servo of the back laser should start to move, which indicates the start of the program. So you can connect now via WIFI.
5. On your smartphone, enable WIFI and connect to the WLAN with the SSID "lazyLegoCar". Then start the app.
6. In the app, there are two different modes of operation:
	-Direct control: Use the buttons to control the steering and driving direction of the car.
	-Path planning: Enter x- and y- coordinates and send it to the car. A positive x-Value means, the destination lies x centimeters to the right of the car. A positive y-Value means, the destination lies y centimeters in front of the car. After you send the coordinates, the car starts driving towards the final destination without your intervention. 
	NOTE: Since the car has no position sensors, all the calculations of the current position of the car rely on measurements and calibrations that have been done with full batteries. So if the car doesn't reach the exact destination, you can either use full batteries or change the constants which determine the speed.


Compiling from source:
=================
It is recommended to do is on a Linux PC.

Clone the repository:
git clone github.com/jmstark/lazyLegoCar
 
Compiling code for arduino:
Load arduino/arduino.ino into the Arduino IDE. Compile and upload the code to the arduino.

Compiling code for raspberry pi:
Go into raspberry directory: cd raspberry.
A makefile is already supplied, so a simple "make" command starts the building process. The application is named "lazyLegoCar".

Compiling the App:
Import the project from the "HelloWorld" folder into eclipse (Android Development Tools and stuff has to be installed).
Eclipse is now able to recompile it.