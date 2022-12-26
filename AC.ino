/*
 * WiFi Microphone program For ESP8266
 * WiFi Configuration File
 * 
 * Author: M. Mahdi K. Kanan
 * WiCardTech Engineering Group
 * 
 * https://wicard.net
 * https://store.wicard.net
 * 
 * https://wicard.ir
 * https://shop.wicard.net
*/

#include "AC.h"

void configPage_AC()
{
  int i;
  if(Server.arg("aps") != NULL) //Access point settings
  {
    for(i = EEP_AP_SSID; i < (EEP_AP_SSID+EEP_AP_SSID_LEN); i++)
    {
      APSsid[i-EEP_AP_SSID] = Server.arg("aps")[i-EEP_AP_SSID];
      EEPROM.write(i, APSsid[i-EEP_AP_SSID]);
    }

    if(Server.arg("app") != NULL)
    {
      for(i = EEP_AP_PASS; i < (EEP_AP_PASS+EEP_AP_PASS_LEN); i++)
      {
        APPassword[i-EEP_AP_PASS] = Server.arg("app")[i-EEP_AP_PASS];
        EEPROM.write(i, APPassword[i-EEP_AP_PASS]);
      }
    }
    else
    {
      APPassword[0] = 0;
      EEPROM.write(EEP_AP_PASS, 0);
    }

    if(Server.arg("sli") != NULL)
    {
      for(i = EEP_SECURE; i < (EEP_SECURE+EEP_SECURE_LEN); i++)
      {
        if(Server.arg("sli")[i-EEP_SECURE] == 0)
        {
          secureLink[i-EEP_SECURE] = 0;
          
          EEPROM.write(i, 0);

          break;
        }
        else if((Server.arg("sli")[i-EEP_SECURE] >= '0' && Server.arg("sli")[i-EEP_SECURE] <= '9')
          || (Server.arg("sli")[i-EEP_SECURE] >= 'A' && Server.arg("sli")[i-EEP_SECURE] <= 'Z')
          || (Server.arg("sli")[i-EEP_SECURE] >= 'a' && Server.arg("sli")[i-EEP_SECURE] <= 'z'))
        {
          secureLink[i-EEP_SECURE] = Server.arg("sli")[i-EEP_SECURE];
          
          EEPROM.write(i, secureLink[i-EEP_SECURE]);
        }
        else
        {
          secureLink[0] = 0;
          
          EEPROM.write(EEP_SECURE, 0);
  
          break;
        }
      }
    }
    else
    {
      secureLink[0] = 0;
      
      EEPROM.write(EEP_SECURE, 0);
    }

    if(Server.arg("hap") != NULL)
      i = 1;
    else
      i = 0;
      
    if(Server.arg("dap") != NULL)
      i |= 2;

    EEPROM.write(EEP_AC_CONFIG, (i&0xff));
    
    EEPROM.commit();

    //const String content = "";
    PGM_P content = PSTR("<html><head><title>AutoConnect</title></head><body>Hot Spot Saved! Please re-start the module.</body></html>");
    
    Server.send(200, "text/html", content);
  }
  else if(Server.arg("sts") != NULL) //station settings
  {
    for(i = EEP_ST_SSID; i < (EEP_ST_SSID+EEP_ST_SSID_LEN); i++)
    {
      STSsid[i-EEP_ST_SSID] = Server.arg("sts")[i-EEP_ST_SSID];
      EEPROM.write(i, STSsid[i-EEP_ST_SSID]);
    }

    if(Server.arg("stp") != NULL)
    {
      for(i = EEP_ST_PASS; i < (EEP_ST_PASS+EEP_ST_PASS_LEN); i++)
      {
        STPassword[i-EEP_ST_PASS] = Server.arg("stp")[i-EEP_ST_PASS];
        EEPROM.write(i, STPassword[i-EEP_ST_PASS]);
      }
    }
    else
    {
      STPassword[0] = 0;
      EEPROM.write(EEP_ST_PASS, 0);
    }
    
    EEPROM.commit();

#ifdef LOG_ENABLE
    Serial.print("ST SSID:");
    Serial.println(STSsid);
      
    Serial.print("ST PASS:");
    Serial.println(STPassword);
#endif
    //const String content = "
    PGM_P content = PSTR("<html><head><meta http-equiv='refresh' content='100; url=/config'><title>Connecting...</title></head><body>Please Wait...</body></html>");
    
    Server.send(200, "text/html", content);
  }
  else if(Server.arg("b") != NULL && Server.arg("p") != NULL && Server.arg("a") != NULL)//rev 1.2
  {
    //bitrate
    if(Server.arg("b")[0] == '0')
    {
      EEPROM.write(EEP_WM_BITRATE, 0);
      ucWMBitrate = 0;
    }
    else if(Server.arg("b")[0] == '1')
    {
      EEPROM.write(EEP_WM_BITRATE, 1);
      ucWMBitrate = 1;
    }
    else //if(Server.arg("b")[0] == '2')
    {
      EEPROM.write(EEP_WM_BITRATE, 2);
      ucWMBitrate = 2;
    }

    //length
    if(Server.arg("p")[0] == '0')
    {
      EEPROM.write(EEP_WM_PACKETLEN, 0);
      ucWMPacketLen = 0;
    }
    else if(Server.arg("p")[0] == '1')
    {
      EEPROM.write(EEP_WM_PACKETLEN, 1);
      ucWMPacketLen = 1;
    }
    else if(Server.arg("p")[0] == '2')
    {
      EEPROM.write(EEP_WM_PACKETLEN, 2);
      ucWMPacketLen = 2;
    }
    else //if(Server.arg("p")[0] == '3')
    {
      EEPROM.write(EEP_WM_PACKETLEN, 3);
      ucWMPacketLen = 3;
    }

    //amplitude
    if(Server.arg("a")[0] == '0')
    {
      EEPROM.write(EEP_WM_AMP, 0);
      ucWMAmp = 0;
    }
    else if(Server.arg("a")[0] == '1')
    {
      EEPROM.write(EEP_WM_AMP, 1);
      ucWMAmp = 1;
    }
    else //if(Server.arg("a")[0] == '2')
    {
      EEPROM.write(EEP_WM_AMP, 2);
      ucWMAmp = 2;
    }
      
    EEPROM.commit();
    
    PGM_P content = PSTR("<html><head><title>Configuration Saved</title></head><body>Saved.</body></html>");
    
    Server.send(200, "text/html", content);
  }
  else
  {
    /* HTBPACHpSIDRA rev 1.3
              H: hidden hotspot=1
              T: temp hotspot=1
              B: bitrate
              P: packet len
              A: amplitude
              C: conntected to SSID [RSSI]*IP*saved SSID*
              H: hotspot SSID*
              p: hotspot password*
              S: secure link*
              I: Linked ip*
              D: ST MAC addr
              R: AP MAC addr
              A: available aps*/

    String content = "";

    if(byte(EEPROM.read(EEP_AC_CONFIG)) & 0x01)
      content += 1;
    else
      content += 0;

    if(byte(EEPROM.read(EEP_AC_CONFIG)) & 0x02)
      content += 1;
    else
      content += 0;

    content += int(ucWMBitrate);
    content += int(ucWMPacketLen);
    content += int(ucWMAmp);

    if(WiFi.status() == WL_CONNECTED)
    {
      content += STSsid;
      content += " [";
      content += WiFi.RSSI();
      content += "]*";
      content += WiFi.localIP().toString();
      content += "*";
    }
    else
      content +="-*-*";

    content += STSsid;
    content += "*";

    content += APSsid;
    content += "*";

    content += APPassword;
    content += "*";

    content += secureLink;
    content += "*";

    byte mac[6];

    WiFi.macAddress(mac);

#ifdef LOG_ENABLE
    Serial.print("MAC: ");
    Serial.print(mac[0],HEX);
    Serial.print(":");
    Serial.print(mac[1],HEX);
    Serial.print(":");
    Serial.print(mac[2],HEX);
    Serial.print(":");
    Serial.print(mac[3],HEX);
    Serial.print(":");
    Serial.print(mac[4],HEX);
    Serial.print(":");
    Serial.println(mac[5],HEX);
#endif

    //ST Mac
    content += String(mac[0],HEX)+":"+String(mac[1],HEX)+":"+String(mac[2],HEX)+":"+String(mac[3],HEX)+":"+String(mac[4],HEX)+":"+String(mac[5],HEX)+"*";

    //AP Mac
    content += String((mac[0]+2),HEX)+":"+String(mac[1],HEX)+":"+String(mac[2],HEX)+":"+String(mac[3],HEX)+":"+String(mac[4],HEX)+":"+String(mac[5],HEX)+"*";

#ifdef LOG_ENABLE
    Serial.println("start scan");
#endif
    
    // WiFi.scanNetworks will return the number of networks found
    int n = WiFi.scanNetworks();

#ifdef LOG_ENABLE
    Serial.println("scan done");
#endif
    
    if(n != 0)
    {
#ifdef LOG_ENABLE
      Serial.print(n);
      Serial.println(" networks found");
#endif

      for(int i = 0; i < n; ++i)
      {
        content += "<div class='ap'><b onclick='ap(this.innerHTML);'>" + WiFi.SSID(i) + "</b><b class='rssi'>" + WiFi.RSSI(i) + "</b></div>";
        // Print SSID and RSSI for each network found
#ifdef LOG_ENABLE
        Serial.print(i + 1);
        Serial.print(": ");
        Serial.print(WiFi.SSID(i));
        Serial.print(" (");
        Serial.print(WiFi.RSSI(i));
        Serial.println(")");
        delay(10);
#endif
      }
    }
#ifdef LOG_ENABLE
    else
    {
      Serial.println("no networks found");
    }
#endif

    Server.send(200, "text/html", content);
  }
}

void reset_AC()
{
  int i;

#ifdef LOG_ENABLE
  Serial.print("EE Reset...");
#endif
  
  STSsid[0] = 0;
  STPassword[0] = 0;
  APSsid[0] = 'W';
  APSsid[1] = 'i';
  APSsid[2] = 'C';
  APSsid[3] = 'a';
  APSsid[4] = 'r';
  APSsid[5] = 'd';
  APSsid[6] = 'M';
  APSsid[7] = 'p';
  APSsid[8] = 0;
  
  APPassword[0] = '1';
  APPassword[1] = '2';
  APPassword[2] = '3';
  APPassword[3] = '4';
  APPassword[4] = '5';
  APPassword[5] = '6';
  APPassword[6] = '7';
  APPassword[7] = '8';
  APPassword[8] = 0;

  EEPROM.write(EEP_AC_KEY, MEM_SAVED);
  EEPROM.write(EEP_ST_SSID, 0);
  EEPROM.write(EEP_ST_PASS, 0);
  
  for(i = EEP_AP_SSID; i < (EEP_AP_SSID+EEP_AP_SSID_LEN); i++)
    EEPROM.write(i, APSsid[i-EEP_AP_SSID]);
    
  for(i = EEP_AP_PASS; i < (EEP_AP_PASS+EEP_AP_PASS_LEN); i++)
    EEPROM.write(i, APPassword[i-EEP_AP_PASS]);
  
  EEPROM.write(EEP_SECURE, 0);
  
  //EEPROM.write(EEP_AC_CONFIG, 0);
  EEPROM.write(EEP_AC_CONFIG, 2);

  //Your EEprom init
  //rev 1.2
  EEPROM.write(EEP_WM_BITRATE, 1);
  ucWMBitrate = 1;
  EEPROM.write(EEP_WM_PACKETLEN, 1);
  ucWMPacketLen = 1;
  EEPROM.write(EEP_WM_AMP, 2);
  ucWMAmp = 0;
  
  EEPROM.commit();
}

void init_AC()
{
#ifdef LOG_ENABLE
  Serial.print("Autoconnect Init!");
#endif
  
  int i;
  
  DisableHotSpot = false;
  
  if(byte(EEPROM.read(EEP_AC_KEY)) == MEM_SAVED)
  {
#ifdef LOG_ENABLE
    Serial.print("EE OK!");
#endif
    for(i = EEP_SECURE; i < (EEP_SECURE+EEP_SECURE_LEN); i++)
      secureLink[i-EEP_SECURE] = EEPROM.read(i);
    
    for(i = EEP_ST_SSID; i < (EEP_ST_SSID+EEP_ST_SSID_LEN); i++)
      STSsid[i-EEP_ST_SSID] = EEPROM.read(i);
      
    for(i = EEP_ST_PASS; i < (EEP_ST_PASS+EEP_ST_PASS_LEN); i++)
      STPassword[i-EEP_ST_PASS] = EEPROM.read(i);
      
    for(i = EEP_AP_SSID; i < (EEP_AP_SSID+EEP_AP_SSID_LEN); i++)
      APSsid[i-EEP_AP_SSID] = EEPROM.read(i);
      
    for(i = EEP_AP_PASS; i < (EEP_AP_PASS+EEP_AP_PASS_LEN); i++)
      APPassword[i-EEP_AP_PASS] = EEPROM.read(i);
    
    //rev 1.2
    ucWMBitrate = EEPROM.read(EEP_WM_BITRATE);
    if(ucWMBitrate > 2)
      ucWMBitrate = 1;
      
    ucWMPacketLen = EEPROM.read(EEP_WM_PACKETLEN);
    if(ucWMPacketLen > 3)
      ucWMPacketLen = 1;
      
    ucWMAmp = EEPROM.read(EEP_WM_AMP);
    if(ucWMAmp > 2)
      ucWMAmp = 0;

    if(STSsid[0] != 0)
    {
      if(byte(EEPROM.read(EEP_AC_CONFIG)) & 0x02)
        WiFi.mode(WIFI_STA);
      else
        WiFi.mode(WIFI_AP_STA);

      WiFi.begin(STSsid, STPassword);

#ifdef LOG_ENABLE
      Serial.print("Connecting To AP:");
      Serial.print(STSsid);
#endif
    }
  }
  else
  {
#ifdef LOG_ENABLE
    Serial.print("EE Init...");
#endif

    reset_AC();
  }
  
  if(byte(EEPROM.read(EEP_AC_CONFIG)) & 0x01)
    WiFi.softAP(APSsid, APPassword, 1, true);
  else
    WiFi.softAP(APSsid, APPassword);

#ifdef LOG_ENABLE
  Serial.print("Autoconnect Init Done!");
#endif
}

void handle_AC()
{
  if(WiFi.status() != WL_CONNECTED)
    Delay10Sec();
    
  if(WiFi.status() != WL_CONNECTED)
  {
    delay(200);
    
    WiFi.mode(WIFI_AP);
    
    delay(200);
    
    if(STSsid[0] != 0)
    {
#ifdef LOG_ENABLE
      Serial.println("Reconnecting");
#endif
      Delay10Sec();
      Delay10Sec();
      Delay10Sec();
      //Delay10Sec();
      //Delay10Sec();
      //Delay10Sec();
#ifdef LOG_ENABLE
      Serial.print("Connecting To AP:");
      Serial.println(STSsid);
#endif
      delay(200);
      WiFi.mode(WIFI_AP_STA);
      delay(200);
      WiFi.begin(STSsid, STPassword);
      delay(200);

      Delay10Sec();
      Delay10Sec();
    }
    DisableHotSpot = false;
  }
  else if(DisableHotSpot == false)
  {
#ifdef LOG_ENABLE
    Serial.println("WiFi connected: " + WiFi.localIP().toString());
#endif

    if(byte(EEPROM.read(EEP_AC_CONFIG)) & 0x02)
      WiFi.mode(WIFI_STA);

    delay(200);

    DisableHotSpot = true;
  }
}
