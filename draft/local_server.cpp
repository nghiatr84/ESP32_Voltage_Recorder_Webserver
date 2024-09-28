#include <Arduino.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>
#include <Wire.h>

float voltage = 0;
// read the input on analog pin 34 (ADC1 pin, ADC2 pins don't work with WiFi on):
int analogInput = 34;

const char* ssid = "MSI";
const char* password = "awhyeawh";

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

void setup() {
  /*
  Some problem in this setup() function causing ESP32 to not read Analog at pin 4, but I don't know why yet
  */
  // Serial port for debugging purposes
  Serial.begin(115200);

  // Initialize SPIFFS
  if(!SPIFFS.begin()){
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while(WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  // Print ESP32 Local IP Address
  Serial.println(WiFi.localIP());

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index.html");
  });
  server.on("/voltage", HTTP_GET, [](AsyncWebServerRequest *request){    
    request->send_P(200, "text/plain", String(voltage).c_str());
  });

  // Start server
  server.begin();
}

void loop() {  
  // read the input on analog pin 34 (ADC1 pin, ADC2 pins don't work with WiFi on):
  int sensorValue = analogRead(analogInput);
  // Convert the analog reading (which goes from 0 - 4095) to a voltage (0 - 3.3V):
  voltage = sensorValue * (3.3 / 4095.0);
  // Read input voltage every 200ms
  delay(200);
}