
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
unsigned long startMillis;           //Reset every 15 seconds
unsigned long currentMillis;          //Reset every loop for Time Counting
const unsigned long period = 15000;  //Upload the data every 15 seconds

/*for collecting and processing data*/
unsigned int RTimesPressed = 0;                //data to read from Windchime and upload to ThingSpeak - how many time my Windchime hit in 15 seconds
unsigned int WTimesPressed = 0;                //data to read from ThingSpeak and play out on Speaker - how many time the other Windchime hit in 15 seconds
unsigned long PlaystartMillis = 0;           //Reset every play periord
unsigned long Playperiod = 0;       //A value to decide how often to play windchime sound for each 15 seconds period

/*speaker*/
#include <SoftwareSerial.h>
#define CMD_PLAY_NEXT 0x01
#define CMD_PLAY_PREV 0x02
#define CMD_PLAY_W_INDEX 0x03
#define CMD_SET_VOLUME 0x06
#define CMD_SEL_DEV 0x09
#define CMD_PLAY_W_VOL 0x22
#define CMD_PLAY 0x0D
#define CMD_PAUSE 0x0E
#define CMD_SINGLE_CYCLE 0x19
#define DEV_TF 0x02
#define SINGLE_CYCLE_ON 0x00
#define SINGLE_CYCLE_OFF 0x01
#define ESP8266_RX D5  // The ESP8266 pin connected to the TX of the Serial MP3 Player module
#define ESP8266_TX D6  // The ESP8266 pin connected to the RX of the Serial MP3 Player module
SoftwareSerial mp3(ESP8266_RX, ESP8266_TX);

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  ThingSpeak.begin(client);
  pinMode(BUILTIN_LED, OUTPUT);
  pinMode(D1, OUTPUT);
    //digitalWrite(BUILTIN_LED, HIGH); 

    /*speaker*/
      mp3.begin(9600);
  delay(500);  // wait chip initialization is complete
  mp3_command(CMD_SEL_DEV, DEV_TF);  // select the TF card
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

void mp3_command(int8_t command, int16_t dat) {
  int8_t frame[8] = { 0 };
  frame[0] = 0x7e;                // starting byte
  frame[1] = 0xff;                // version
  frame[2] = 0x06;                // The number of bytes of the command without starting byte and ending byte
  frame[3] = command;             //
  frame[4] = 0x00;                // 0x00 = no feedback, 0x01 = feedback
  frame[5] = (int8_t)(dat >> 8);  // data high byte
  frame[6] = (int8_t)(dat);       // data low byte
  frame[7] = 0xef;                // ending byte
  for (uint8_t i = 0; i < 8; i++) {
    mp3.write(frame[i]);
  }
}
