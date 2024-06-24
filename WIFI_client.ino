#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>

#include <ESP8266WiFiMulti.h>
ESP8266WiFiMulti WiFiMulti;

const char* ssid = "ESP8266-Access-Point";
const char* password = "123456789";

//Your IP address or domain name with URL path
const char* server12 = "http://192.168.4.1/12";
const char* server13 = "http://192.168.4.1/13";
const char* server14 = "http://192.168.4.1/14";
const char* server15 = "http://192.168.4.1/15";
const char* server16 = "http://192.168.4.1/16";

int data12;
int data13;
int data14;
int data15;
int data16;

unsigned long previousMillis = 0;
const long interval = 50; 


void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    }
    Serial.println("Connected to WiFi");
pinMode(12,OUTPUT);
pinMode(13,OUTPUT);
pinMode(14,OUTPUT);
pinMode(15,OUTPUT);
pinMode(16,OUTPUT);
}


void loop() {
  unsigned long currentMillis = millis();
  
  
  if(currentMillis - previousMillis >= interval) {
     // Check WiFi connection status
    if ((WiFiMulti.run() == WL_CONNECTED)) {

      
      data12 = httpGETRequest(server12).toInt(); 
      data13 = httpGETRequest(server13).toInt(); 
      data14 = httpGETRequest(server14).toInt(); 
      data15 = httpGETRequest(server15).toInt(); 
      data16 = httpGETRequest(server16).toInt(); 
      Serial.print(data12);
      Serial.print(data13);
      Serial.print(data14);
      Serial.print(data15);
      Serial.println(data16);
          
          
          if (data12 == 0){
            digitalWrite(12, LOW);
//            Serial.println("bbbb");
          }
          else{
            digitalWrite(12, HIGH);
  //        Serial.println("cccc");
          }

          
          if (data13 == 0){
            digitalWrite(13, LOW);
                   }
          else{
            digitalWrite(13, HIGH);
                    }

          
          if (data14 == 0){
            digitalWrite(14, LOW);
                   }
          else{
            digitalWrite(14, HIGH);
                    }

                    
          if (data15 == 0){
            digitalWrite(15, LOW);
                   }
          else{
            digitalWrite(15, HIGH);
                    }

                    
          if (data16 == 0){
            digitalWrite(16, LOW);
                   }
          else{
            digitalWrite(16, HIGH);
                    }
      
      previousMillis = currentMillis;
    }
    else {
      Serial.println("WiFi Disconnected");
                   }
  }
}



String httpGETRequest(const char* serverName) {
  WiFiClient client;
  HTTPClient http;
    
  // Your IP address with path or Domain name with URL path 
  http.begin(client, serverName);
  
  // Send HTTP POST request
  int httpResponseCode = http.GET();
  
  String payload = "--"; 
  
  if (httpResponseCode>0) {
//    Serial.print("HTTP Response code: ");
//    Serial.println(httpResponseCode);
    payload = http.getString();
  }
  else {
//    Serial.print("Error code: ");
//    Serial.println(httpResponseCode);
  }
  // Free resources
  http.end();

  return payload;
}
