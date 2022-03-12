#ifndef __Main_h
#define __Main_h


#include <Arduino.h>
#include <ArduinoOTA.h>
#include <EEPROM.h>

#include <NTPClient.h>

#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <user_interface.h>

// #include <MySQL_Connection.h>
// #include <MySQL_Cursor.h>

#include <PubSubClient.h>

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>



//#define DEBUG_Prog
//#define SOLAR

#define pinRelais D5
#define pinLed D6
#define pinRain D7

#define UREF 3.15

#define TEMP_AV_SIZE 150

#define INTERVALL_1S  1000
#define INTERVALL_10S 10000
#define INTERVALL_1m  60000
#define INTERVALL_10m 600000
#define INTERVALL_1h  3600000
#define INTERVALL_24h 86400000

#define mm_TickRain 0.2974

extern const char* SSID_1;
extern const char* SSID_2;
extern const char* SSID_PASSWORD;

extern const char* mqtt_Broker_IP;
extern const char* mqtt_User;
extern const char* mqtt_Pw;
extern const char* mqtt_ClientId;

extern IPAddress mySqlServer;
extern char  mySqlUser[];
extern char  mySqlPw[];


extern const int32_t utcOffsetInSeconds;  

extern WiFiUDP ntpUDP;
extern NTPClient timeClient;

//ESP8266WiFiMulti wifiMulti;
extern WiFiClient WetterStation_Client;

extern PubSubClient mqttClient;

// extern MySQL_Connection conn;

extern Adafruit_BME280 BME280;

extern uint8_t I2C_Error;

extern float Temp_Av_10m[10];
extern float Hum_Av_10m[10];
extern float Pres_Av_10m[10];
extern uint8_t Av_10m_Index;

extern int16_t Temp_Av;
extern float fTemp_Av_Day;
extern uint16_t Temp_Av_Count;

extern uint16_t Speed;
extern uint32_t millisCur_Wind;
extern uint32_t millisPrev_Wind;
extern uint8_t led_Status;



extern uint32_t unixTime;
extern uint32_t millis_1S;
extern uint32_t millis_10S;
extern uint32_t millis_1m;
extern uint32_t millis_10m;
extern uint32_t millis_1h;
extern uint32_t millis_24h;
extern uint32_t days_30d;

extern uint8_t rain_hour;
extern uint8_t rain_day;
extern uint8_t pinRain_Status_Old; 
extern uint32_t rain_millis_old;



extern float fWind_Speed;
extern float fWind_Dir;





void vDelay(uint16_t d);

void writeDatabase_VBatt();
void writeDatabase_10m();
void writeDatabase_1h();
void writeDatabase_24h();

void Ticker_1s();
void Ticker_10s();
void Ticker_1m();
void Ticker_10m();
void Ticker_1h();
void Ticker_24h();
void Ticker_30d();
void Ticker();

void I2C_Scanner();

void getBMESensorData(float &temp , float &pres , float &hum);
void getRainData();
float getVBatt();
uint16_t getWindCounter();
uint8_t getWindDirection();

uint8_t readEEprom8(uint16_t reg);
uint16_t readEEprom16(uint16_t reg);
void writeEEprom8(uint16_t reg , uint8_t value);
void writeEEprom16(uint16_t reg , uint16_t value);

void WifiON();
void WifiOFF();
byte checkWlanConnection();
void connectWifi();
void setupOTA();
uint8_t mqtt_Connect();
void mqtt_Callback(char* topic , byte* payload , int length);


#endif