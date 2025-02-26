# IoT-Based-Home-Automation 
IoT-based Home Automation Project using PICSimLab, Arduino IDE, Free Virtual Ports, and Blynk IoT involves controlling devices like LEDs through an LDR sensor, a serial tank, a heater, and a cooler. Below is a structured approach to implementing this project.
This system will:

Use PICSimLab to simulate the microcontroller circuit.
Code the microcontroller using Arduino IDE.
Establish communication using Free Virtual Ports for serial data transfer.
Utilize Blynk IoT for remote control and monitoring.

Software

PICSimLab (Microcontroller Simulation)
Arduino IDE (Coding the microcontroller)
Free Virtual Serial Ports (Communication)
Blynk IoT (Remote control via smartphone app)
Hardware (Simulated in PICSimLab)

Microcontroller: PIC18F series or Arduino (Simulated)
LED (Controlled via LDR sensor)
LDR Sensor (To detect light intensity)
Heater & Cooler (Controlled based on temperature input)
Serial Tank (For liquid level monitoring)
# Step for Procedure:

1) Download PicsimLab, VirtualFreePorts, ArduinoIDE, BlynkIoT App
2) In VirtualFreePorts App, Create a network bridge (in my case: its between COM1 - COM2), make connection as per availability of ports
3) Source Code is provided
4) In ArduinoIDE, Go to Sketch then click on Export compiled Binary.
5) Sketch is also provided by extension pcf in the above files..........
6) Made the connection as per code
7) now load the hex file in the picsimlab
8) its ready to go........................ 
