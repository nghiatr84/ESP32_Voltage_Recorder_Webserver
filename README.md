# Retrieving Triboelectric Device Voltage Level

<!-- ![A gif which demonstrates the device](./demonstration.gif) -->
<img alt="A gif which demonstrates the device" src="./demonstration.gif" width="600" height="375" />

## Project Documentation
### Description: 
A device that can be used to measure Alternating Current (AC) voltage level generated from a triboelectric device by rectifying the current to a Direct Current (DC) and read the voltage level through an Analog-Digital Converter (ADC) pin of the microcontroller Espressif ESP32-WROOM-32E. The hardware will be accompanied by a web server which visualises and handles recorded data.

Project Managers: Dr Ramyah Gowrishankar, Prof Yu Xiao

Project Owner: Nghia Tran

### Timeline:
- Prototype version 1: 24.09.2023.
- Prototype version 2: 08.10.2023.
- Final product: 15.10.2023
- Documentation and Learning Log: 20.10.2023.
### Scope: 
The scope of this project includes creating a fully functional prototype and a printed circuit board (PCB) schematic for further development.
### Goals and Objectives:
- Prototype version 1: Develop a fully functional embedded prototype.
	- Objective 1.1: The device can read the voltage level accurately.
	- Objective 1.2: The device can operate within a desired response time.
- Prototype version 2: Develop a fully functional web server that can handle recorded data.
	- Objective 2.1: The web server can visualise the recorded data.
	- Objective 2.2: The web server can instruct the microcontroller to start/stop recording data.
	- Objective 2.3: The web server can control the microcontroller recording speed.
	- Objective 2.4: The web server can download the recorded data.
### Process:
The process of developing the Objective step-by-step.
#### Prototype version 1
##### Objective 1.1:
To read the voltage, we use Arduino analogRead() function. The ESP32 returns a range from 0 - 4095 for the raw analogRead() values, and we need to map those discrete values to volts. According to the manufacturer Espressif, ESP32-WROOM-32E can read from 0 – 3.3V maximum (https://docs.espressif.com/projects/esp-idf/en/v4.2/esp32/api-reference/peripherals/adc.html), so we are mapping 0 - 4095 to 0 – 3.3 by dividing the received value by 
$${3.3 \over 4095}$$
Following Figure 1 schematic, we created the rectifying circuit needed to change AC from the triboelectric device to DC. Then, the 1nF capacitor is used to stabilise the input. The microcontroller ESP32-WROOM-32E can only read inputs up to 3.3V. Furthermore, the triboelectric device produces high voltage but low current electricity, and the effect happens very fast (as fast as the friction time of the cloth components). Therefore, we place a 4700 Ohms resistor to scale down the input voltage. Through experiments, we have reached a conclusion that a 4700 Ohms resistor works best for the ESP32 microcontroller to read the voltage as accurate as a multi-meter. 
 
 ![image](https://github.com/nghiatr84/ESP32_Voltage_Recorder_Webserver/assets/132190213/7b285508-1b00-4d70-9711-cd1e4ed4c8af)

Figure 1: Rectifying circuit and microcontroller pinout schematic

![PCB_3D_batteryUpdate](https://github.com/nghiatr84/ESP32_Voltage_Recorder_Webserver/assets/132190213/fadd8315-6a90-4550-a15b-60fe49abb843)

Figure 2: The model PCB 3D model

##### Objective 1.2:
We must use a capacitor to stabilise the current, otherwise there would be too much noise to read from. The problem with response time is if we use a capacitor that is too high of a Farad value, it will take longer time to charge and discharge. On the other hand, too low of a Farad value would cause the microcontroller to read in more noise. The aim here is to find which capacitor works best. The triboelectric device voltage level can be varied vastly according to when we try to record, so the most practical way to find the best capacitor is by experimenting with a wide range of them. Finally, the best capacitor for this triboelectric device is 1nF capacitor.
#### Prototype version 2
For Objective 2.1, we use highcharts.js library for making the graph and html for web page template. The code in the appendices follows a guide from https://randomnerdtutorials.com/esp32-esp8266-plot-chart-web-server/ with some modifications for reading the voltage. The following Objectives are done using ESPAsyncWebServer library for the web server for Arduino frame and JavaScript + HTML for the client. Details can be found in the code appendices.
### Components used: 
	Espressif ESP32-DevKit-V4 microcontroller: 1 piece
	1N4148 diode: 4 pieces
	4700 Ohms resistor: 1 piece
	1nF capacitor: 1 piece
	Jumper wires
	Breadboard: 1 piece
	Alligator clips: 2 pieces
#### IDEs & libraries used:
- VSCode with Platform.io plugin
- Arduino IDE with
- 	LittleFS.h library for file handling
- 	Wifi.h and ESPAsyncWebServer.h libraries for WiFi AccessPoint/Station handling
- 	time.h library for time managing
- 	Arduino.h library frame to handle ESP32 microcontroller
