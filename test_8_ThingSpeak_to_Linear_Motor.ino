/*Include two channels & API Keys*/
/*Update to include both sensor and motor configurations*/

/*for wifi & ThingSpeak connection*/
#include "ThingSpeak.h"
#include "secrets.h"
#include <ESP8266WiFi.h>
unsigned long myChannelNumber = SECRET_CH_ID;
const char * myWriteAPIKey = SECRET_WRITE_APIKEY;
const char * myReadAPIKey = SECRET_READ_APIKEY;
char ssid[] = SECRET_SSID;   // your network SSID (name)
char pass[] = SECRET_PASS;   // your network password
//int keyIndex = 0;            // your network key index number (needed only for WEP)
WiFiClient  client;

/*for Time Counting to exchange Sensor data*/
unsigned long startMillis;           //Reset every 5 seconds
unsigned long currentMillis;          //Reset every loop for Time Counting
const unsigned long period = 5000;  //Upload the data every 5 seconds

/*for collecting and processing data*/
unsigned int RTimesPressed = 0;                //data to read from Sensor and upload to ThingSpeak - how many time my Windchime hit in 5 seconds
unsigned int WTimesPressed = 0;                //data to read from ThingSpeak and play on Motor - how many time the other Windchime hit in 5 seconds
unsigned long PlaystartMillis = 0;           //Reset every play periord
unsigned long Playperiod = 0;       //A value to decide how often to play windchime sound for each 15 seconds period

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  ThingSpeak.begin(client);
  pinMode(BUILTIN_LED, OUTPUT);
  pinMode(D1, OUTPUT);
    //digitalWrite(BUILTIN_LED, HIGH); 
}

void loop() {    
  // Connect or reconnect to WiFi
  if (WiFi.status() != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(SECRET_SSID);
    while (WiFi.status() != WL_CONNECTED) {
      WiFi.begin(ssid, pass); // Connect to WPA/WPA2 network. Change this line if using open or WEP network
      digitalWrite(BUILTIN_LED, LOW);
      Serial.print(".");
      delay(5000);
    }
    Serial.println("\nConnected.");
    digitalWrite(BUILTIN_LED, HIGH); 
  }

/*for each 15 seconds, upload to and read from ThingSpeak*/
currentMillis = millis(); // update time
  if (currentMillis - startMillis >= period)  {
    //Serial.println(RTimesPressed);
    startMillis = currentMillis;  //save the start time of every period (15s).
    // Write value to Field 1 of a ThingSpeak Channel
  /*int httpCode = ThingSpeak.writeField(myChannelNumber, 1, TimesPressed, myWriteAPIKey);
    TimesPressed = 0; */
    //Read from Field 1 
    WTimesPressed = ThingSpeak.readIntField(myChannelNumber, 1, myReadAPIKey);
    Serial.println(WTimesPressed);
    if (WTimesPressed > 0){
    Playperiod = long(period / WTimesPressed);
    Serial.println(Playperiod);
    PlaystartMillis = random (currentMillis + 200, currentMillis + Playperiod);
    Serial.println(PlaystartMillis);    }
  }

    //Play MUSICCCC!!
      if ((WTimesPressed > 0 ) && (currentMillis - PlaystartMillis >= 0 ) ){
        Serial.println(0000);
      //mp3_command(CMD_PAUSE, 0x0000);
      mp3_command(CMD_PLAY, 0x0000);
      WTimesPressed = WTimesPressed - 1;
      PlaystartMillis = random (currentMillis +200, currentMillis + Playperiod);
      Serial.println(PlaystartMillis);
  }

}
