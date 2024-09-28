#include <Arduino.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include "littleFSFunc.h"
#include "time.h"

void printLocalTime();

// getTime from client
time_t rawtime;
struct tm* info = localtime(&rawtime);

float voltage = 0;
// read the input on analog pin 34 (ADC1 pin, ADC2 pins don't work with WiFi on):
int analogInput = 35;

// Delay time in ms
int delayTime = 5000;

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

// true to start record
bool record = false;

#define FORMAT_LITTLEFS_IF_FAILED true

// On server not found
void notFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "Not found");
}

void setup() {
  // Serial port for debugging purposes
  Serial.begin(115200);

  // Initialize LittleFS
  if(!LittleFS.begin(FORMAT_LITTLEFS_IF_FAILED)){
      Serial.println("LittleFS Mount Failed");
      return;
   }
   else{
       Serial.println("Little FS Mounted Successfully");
   }

  // Check if the file already exists to prevent overwritting existing data
  bool fileexists = LittleFS.exists("/data.txt");  
  if(!fileexists) {
    Serial.println("Log file doesn’t exist");  
    Serial.println("Creating Log file...");
    // Create File and add header
    writeFile(LittleFS, "/data.txt", "ESP32 voltage recorder\r\n");
  }
  else {
    Serial.println("Log file exists");
  }

  // Check files in LittleFS
  listDir(LittleFS, "/", 1);  

  // Connect to Wi-Fi. Uncomment below to connect to external Wi-Fi
  /*
  const char* ssid = "MSI";
  const char* password = "awhyeawh";
  WiFi.begin(ssid, password);
  while(WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }  
  Serial.println(WiFi.localIP());  // Print ESP32 Local IP Address
  */
 
  // Setting ESP32 as Wi-Fi AP
  // Replace with your network credentials
  const char* ssid     = "ESP32-Access-Point";
  const char* password = "123456789";
  // Connect to Wi-Fi network with SSID and password
  Serial.println("Setting AP (Access Point)…");
  // Remove the password parameter, if you want the AP (Access Point) to be open
  WiFi.softAP(ssid, password);

  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(LittleFS, "/index.html");    
  });
  // Route for loading Highcharts.js
  server.on("/highcharts.js", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(LittleFS, "/highcharts.js");
  });
  server.on("/voltage", HTTP_GET, [](AsyncWebServerRequest *request){    
    request->send_P(200, "text/plain", String(voltage).c_str());
  });
  // Route for data.txt
  server.on("/data.txt", HTTP_GET, [](AsyncWebServerRequest *request){    
    if(!LittleFS.exists("/data.txt")) {
      Serial.println("Cannot download. Log file doesn’t exist");  
      request->send_P(400, "text/plain", "Recordings has been deleted. Start new recording to download log.");
    }
    else
      request->send(LittleFS, "/data.txt");    
  });
  // Route for start recording
  server.on("/start", HTTP_GET, [](AsyncWebServerRequest *request){
    record = true;
    Serial.println("Recording status: true");
    request->send_P(200, "text/plain", "Recording starts");
  });
  // Route for stop recording
  server.on("/stop", HTTP_GET, [](AsyncWebServerRequest *request){
    record = false;
    Serial.println("Recording status: false");
    request->send_P(200, "text/plain", "Recording stops");
  });
  // Route for delete previous recordings
  server.on("/delete", HTTP_GET, [](AsyncWebServerRequest *request){
    int status = deleteFile(LittleFS, "/data.txt");
    if(status == 1)
      request->send_P(200, "text/plain", "Deleted previous recordings");
    else if(status == -1)
      request->send_P(404, "text/plain", "Couldn't delete previous recordings. File not exists.");
    else
      request->send_P(400, "text/plain", "Couldn't delete previous recordings. Unknown reason.");
  });
  // Route for setting delay time
  server.on("/delay", HTTP_GET, [](AsyncWebServerRequest *request){    
    // GET delayTime value on <ESP_IP>/get?delayTime=<input>
    if(request->hasParam("delayTime")){
      delayTime = request->getParam("delayTime")->value().toInt();
      Serial.printf("Changed delay time to %d ms\n", delayTime);
      request->send_P(200, "text/plain", "Successfully changed delay time.");
    }    
    else request->send_P(204, "text/plain", "Nothing changed.");
  });
  server.on("/time", HTTP_GET, [](AsyncWebServerRequest *request){        
    if(request->hasParam("time")){
      rawtime = request->getParam("time")->value().toInt();
      // Serial.println(rawtime);
      info = localtime(&rawtime);
      // printLocalTime();
      request->send_P(200, "text/plain", "Time updated");
    }
    else request->send_P(500, "text/plain", "Nothing changed. Couldn't receive time.");
  });

  server.onNotFound(notFound);
  // Start server
  server.begin();  
}

void loop() { 
  // reset voltage variable when not reading 
  voltage = 0;
  if(record) {
    // read the input on analog pin 34 (ADC1 pin, ADC2 pins don't work with WiFi on):
    int sensorValue = analogRead(analogInput);
    // Convert the analog reading (which goes from 0 - 4095) to a voltage (0 - 3.3V):
    voltage = sensorValue * (3.3 / 4095.0);  

    // Get time    
    printLocalTime();
    char now[20];
    strftime(now, 20, "%H:%M:%S_%d_%m_%Y", info);

    // Appead to data.txt
    appendFile(LittleFS, "/data.txt", (String(now) + "_" + String(voltage) + "V\r\n").c_str()); //Append data to the file
    // Read from data.txt
    readFile(LittleFS, "/data.txt"); // Read the contents of the file
    
    // Read input voltage every %delayTime% ms
    delay(delayTime);
  }
}

void printLocalTime(){
  Serial.println(info, "%A, %B %d %Y %H:%M:%S");
}