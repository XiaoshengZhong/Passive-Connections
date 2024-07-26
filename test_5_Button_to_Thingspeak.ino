//count how many times a button is pressed in 15 seconds and upload to
//a ThingSpeak Channel
#include <ezButton.h>
ezButton button(D1);

unsigned long startMillis;  //some global variables available anywhere in the program
unsigned long currentMillis;
const unsigned long period = 15000;

int TimesPressed = 0;

#include "ThingSpeak.h"
#include "secrets.h"

unsigned long myChannelNumber = SECRET_CH_ID;
const char * myWriteAPIKey = SECRET_WRITE_APIKEY;

#include <ESP8266WiFi.h>

char ssid[] = SECRET_SSID;   // your network SSID (name)
char pass[] = SECRET_PASS;   // your network password
int keyIndex = 0;            // your network key index number (needed only for WEP)
WiFiClient  client;


void setup() {
  Serial.begin(9600);
  startMillis = millis();
  WiFi.mode(WIFI_STA);
  ThingSpeak.begin(client);
  pinMode (D2, OUTPUT);
  analogWrite (D2, 0);
}

void loop() {
  button.loop(); // MUST call the loop() function first
  if (button.isPressed()) {
    TimesPressed = TimesPressed + 1;
  }
  currentMillis = millis();


  // Connect or reconnect to WiFi
  if (WiFi.status() != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(SECRET_SSID);
    while (WiFi.status() != WL_CONNECTED) {
      WiFi.begin(ssid, pass); // Connect to WPA/WPA2 network. Change this line if using open or WEP network
      analogWrite(D2, 150);
      Serial.print(".");
      delay(5000);
    }
    Serial.println("\nConnected.");
    analogWrite(D2, 0);
  }

  
  if (currentMillis - startMillis >= period)
  {
    Serial.println(TimesPressed);
    startMillis = currentMillis;  //IMPORTANT to save the start time of the current LED state.

    // Write value to Field 1 of a ThingSpeak Channel
  int httpCode = ThingSpeak.writeField(myChannelNumber, 1, TimesPressed, myWriteAPIKey);
    
    TimesPressed = 0;
    
  }

  

  
}
