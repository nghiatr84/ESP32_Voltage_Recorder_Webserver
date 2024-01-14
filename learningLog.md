# Retrieving Triboelectric Device Voltage Level
## Project Learning Log
### After the project, I have:
- Furthered my understanding in developing embedded device using ESP32 microcontroller,
- Furthered my understanding in developing web server using ESP32 microcontroller,
- Learned to send requests to web server using HTML and JavaScript,
- Learned to visualise data as graphs using JavaScript,
- Learned the basic concepts of triboelectricity.
I can use the knowledge I have gathered from this project to help me on my academic journey and
career in the fields of embedded systems, programming, back-end and front-end web development, and
smart wearables.
### The challenges I faced making this device include:
- How to enhance the device’s responsiveness. This entirely depends on how fast we would want
the device to store and discharge the energy. Hence, changing the capacitor would make sense
in this case.
- Making the voltage small enough so the microcontroller can read, while maintaining the current.
Because the current is so small, a decent voltage divider will dissipate all the current into heat.
Using smaller resistance resistors is also not an option since it will cause a short circuit. The
solution I have come up with is to use only one resistor so that the voltage drops but the current
went through enough for the ADC of ESP32 to read. I have tried a lot of resistors and recheck the
read voltage from ESP32 with a multi-meter to ensure the accuracy of the voltage being
recorded.
- ADC2 pins of an ESP32 would not work with Wi-Fi turned on, and I must switch to ADC1 pins
instead. This is mentioned in the documentation of the ESP32; however, it took me a lot of time
to find out the problem.
- Making the web server and the web client was a challenge themselves. I consulted internet
forums extensively and spent a lot of time figuring out, but eventually managed to finish it.
