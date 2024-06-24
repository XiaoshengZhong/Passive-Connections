/*
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/esp8266-nodemcu-client-server-wi-fi/
  
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files.
  
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
*/

// Import required libraries
#include <ESP8266WiFi.h>
#include "ESPAsyncWebServer.h"

// Set your access point network credentials
const char* ssid = "ESP8266-Access-Point";
const char* password = "123456789";

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

String read12() {
  return String(digitalRead(12));
}

String read13() {
  return String(digitalRead(13));
}

String read14() {
  return String(digitalRead(14));
}

String read15() {
  return String(digitalRead(15));
}

String read16() {
  return String(digitalRead(16));
}

void setup(){
  // Serial port for debugging purposes
  Serial.begin(115200);
  Serial.println();
  
  // Setting the ESP as an access point
  Serial.print("Setting AP (Access Point)…");
  // Remove the password parameter, if you want the AP (Access Point) to be open
  WiFi.softAP(ssid, password);

  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);

  server.on("/12", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", read12().c_str());
  });
  server.on("/13", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", read13().c_str());
  });
  server.on("/14", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", read14().c_str());
  });
  server.on("/15", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", read15().c_str());
  });
  server.on("/16", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", read16().c_str());
  });
  
  
  // Start server
  server.begin();
}
 
void loop(){
  
}
