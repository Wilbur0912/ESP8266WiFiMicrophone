/*
 * WiFi Microphone Arduino program For ESP8266
 * Rev 1.3
 * 
 * Tested with esp8266 board 3.0.2 and Ai Thinker ESP12 module (ESP8266MOD)
 * Compatible with ESP8266 NodeMCU
 * 
 * Main File
 * 
 * Author: M. Mahdi K. Kanan
 * WiCardTech Engineering Group
 * 
 * English:
 * https://wicard.net
 * https://store.wicard.net
 * 
 * Persian:
 * https://wicard.ir
 * https://shop.wicard.net
*/

#include <time.h>
#include <Arduino.h>
#include "EEPROM.h"

#include "AC.h"

#ifdef UPDATE_IP
#include <ESP8266HTTPClient.h>
#endif

unsigned int  ipTimer;
unsigned char buff[2000];
unsigned long int  ulTimer1;
unsigned long int  ulTimer2;

#ifdef UPDATE_IP
HTTPClient    http;    //Declare object of class HTTPClient
#endif

void setup()
{
  int i;

  ipTimer = 0;
  
  WiFi.mode(WIFI_OFF);
  pinMode(2, OUTPUT); //blue led
         
  //blink for 1 second
  digitalWrite(2, LOW);
  delay(1000);
  digitalWrite(2, HIGH);

  DisableHotSpot = false;
  
  Serial.begin(115200);
  Serial.setDebugOutput(false);
  
#ifdef LOG_ENABLE
  Serial.println(" ");
  Serial.println("** WiCardTech **");
  Serial.println("** WiCard WiFi Microphone For ESP8266 **");
  Serial.println("** https://store.wicard.net **");
#endif

  //0-161 is allocated for AutoConnect (162 bytes)
  //162-255 is free for the program (94 bytes)
  EEPROM.begin(256);
  
  delay(1000);
  
  init_AC();
  
  // Configuring Internal Pages
  if(secureLink[0] == 0)
  {
    Server.on("/", rootPage);
    Server.on("/send", streamPage);
  }
  else
  {
    Server.on("/"+((String)secureLink)+"/", rootPage);
    Server.on("/"+((String)secureLink), rootPage);//added-rev 1.2
    Server.on("/"+((String)secureLink)+"/send", streamPage);//fixed-rev 1.2
  }
  
  delay(1000);
  
  Server.begin();
#ifdef LOG_ENABLE
  Serial.println("HTTP server started");
#endif

  //Your Setup code
  
#ifdef LOG_ENABLE
  Serial.println("Setup Done!");
#endif

  delay(1000);
}

//Main Loop!
void Delay10Sec()
{
  unsigned int i, j;

  unsigned long mic;
  
  for(i = 0; i < LOOP_10_S; i++)
  {
    Server.handleClient();
    
    //Your loop code
  }
}

void loop()
{
  Delay10Sec();
#ifdef LOG_ENABLE
  Serial.print("-");
#endif
  handle_AC();

#ifdef UPDATE_IP
  if(WiFi.status() == WL_CONNECTED)
  {
    if(ipTimer == 100)
    {
      http.begin("http://Your-Website");
      ipTimer = http.GET();
#ifdef LOG_ENABLE
      Serial.println("Updating IP");
      if(ipTimer>0)
      {
        Serial.print("HTTP Response code: ");
        Serial.println(ipTimer);
        Serial.println(http.getString());
      }
      else
      {
        Serial.print("Error code: ");
        Serial.println(ipTimer);
      }
#endif
      ipTimer = 0;
      http.end();  //Close connection
    }
    else
      ipTimer++;
  }
#endif

  //rev 1.3
  if(digitalRead(2) == LOW)
    digitalWrite(2, HIGH);
  else
    digitalWrite(2, LOW);
}
