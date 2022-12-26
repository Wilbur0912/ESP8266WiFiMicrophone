/*
 * WiFi Microphone program For ESP8266
 * WiFi Configuration Header File
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

#ifndef ac_H_
#define ac_H_

#define LOG_ENABLE
//#define UPDATE_IP
#define LOOP_10_S 5500000//2800000
#define MEM_SAVED   0xAB

//rev 1.3
#define AUDIO_80KBPS_TIMING     105
#define AUDIO_90KBPS_TIMING     85
#define AUDIO_100KBPS_TIMING    70

#define PGM_P       const char *
#define PGM_VOID_P  const void *
#define PSTR(s) (__extension__({static const char __c[] PROGMEM = (s); &__c[0];}))

//0-161 is allocated for AutoConnect (162 bytes)
//162-255 is free for the program (94 bytes)
#define EEP_AC_KEY        0
#define EEP_ST_SSID       1
#define EEP_ST_PASS       33
#define EEP_AP_SSID       65
#define EEP_AP_PASS       97
#define EEP_SECURE        129
#define EEP_AC_CONFIG     161

//rev 1.2
#define EEP_WM_BITRATE    162
#define EEP_WM_PACKETLEN  163
#define EEP_WM_AMP        164

#define EEP_ST_SSID_LEN   32
#define EEP_ST_PASS_LEN   32
#define EEP_AP_SSID_LEN   32
#define EEP_AP_PASS_LEN   32
#define EEP_SECURE_LEN    32

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

#include "EEPROM.h"

ESP8266WebServer  Server;

bool DisableHotSpot;

char STSsid[EEP_ST_SSID_LEN]; //1-32
char STPassword[EEP_ST_PASS_LEN]; //33-64
char APSsid[EEP_AP_SSID_LEN]; //65-96
char APPassword[EEP_AP_PASS_LEN]; //97-128
char secureLink[EEP_SECURE_LEN]; //129-160

//rev 1.2
unsigned char ucWMBitrate;
unsigned char ucWMPacketLen;
unsigned char ucWMAmp;

void configPage_AC();
void init_AC();
void handle_AC();

#endif // INCLUDED
