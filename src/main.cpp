#include <main.h>

const char* SSID_1 = "Weffling";
const char* SSID_2 = "Weffling_Outdoor";
const char* SSID_PASSWORD = "86508912437821699676";

const char* mqtt_Broker_IP = "192.168.178.59" ;
const char* mqtt_User = "Mosquitto";
const char* mqtt_Pw   = "-Sam!993+";
const char* mqtt_ClientId = "Wetterstation_Mqtt";

IPAddress mySqlServer(192,168,178,59);
char  mySqlUser[] = "Raspberry";
char  mySqlPw[]   = "R$eT}m8#)o,Q";


const int32_t utcOffsetInSeconds = 0;  

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP , "pool.ntp.org" , utcOffsetInSeconds);

//ESP8266WiFiMulti wifiMulti;
WiFiClient WetterStation_Client;

PubSubClient mqttClient(mqtt_Broker_IP,  1883 , mqtt_Callback ,  WetterStation_Client);



MySQL_Connection conn(&WetterStation_Client);

Adafruit_BME280 BME280;


uint8_t I2C_Error = 0;

float Temp_Av_10m[10];
float Hum_Av_10m[10];
float Pres_Av_10m[10];
uint8_t Av_10m_Index = 0;


int16_t Temp_Av;
float fTemp_Av_Day;
uint16_t Temp_Av_Count;

uint16_t Speed = 0;
uint32_t millisCur_Wind = 0;
uint32_t millisPrev_Wind = 0;
uint8_t led_Status = 0;



uint32_t unixTime   = 0;
uint32_t millis_1S  = 0;
uint32_t millis_10S = 0;
uint32_t millis_1m  = 0;
uint32_t millis_10m = 0;
uint32_t millis_1h  = 0;
uint32_t millis_24h = 0;
uint32_t days_30d   = 0;

uint8_t rain_hour  = 0;
uint8_t rain_day   = 0;
uint8_t pinRain_Status_Old = 0; 
uint32_t rain_millis_old = 0;

float fTemp_Average = 0.0;
float fTemp_Average_10m = 0.0;
float fTemp = 0.0;
float fPres_Average_10m = 0.0;
float fPres = 0.0;
float fHum_Average_10m = 0.0;
float fHum  = 0.0;

char cTemp[10]  = "";
char cPres[10]  = "";
char cHum[10]   = "";


float fWind_Speed = 0.0;
float fWind_Dir  = 0.0;


//Delay Funktion------------------------------------------------------------------------------------------------------------------------
void vDelay(uint16_t d){
  for(uint16_t i = 0; i < d ; i++){
    getRainData();
    delay(1);
  }

}

//-------------------------------Setup-----------------------------------------------------------------------------------------------------
void setup() {
  Serial.begin(115200);

  pinMode(pinLed,OUTPUT);
  pinMode(pinRelais,OUTPUT);
  pinMode(LED_BUILTIN,OUTPUT);
  pinMode(pinRain , INPUT);

  digitalWrite(LED_BUILTIN , 1);

  //wifiMulti.addAP(SSID_1,SSID_PASSWORD);
  //wifiMulti.addAP(SSID_2,SSID_PASSWORD);

  connectWifi();
  checkWlanConnection();
  
  setupOTA();

  mqtt_Connect();

  timeClient.begin();
  timeClient.update();

  unixTime = timeClient.getEpochTime();
  //mqttClient.publish("Wetterstation/time" , unixTime);

  Wire.begin();

  #ifdef DEBUG_Prog
    I2C_Scanner();
  #endif
 
  Serial.println("Connecting Sensor :");
  
  uint8_t Sensor_Status = BME280.begin(0x76);
  if(Sensor_Status)
    Serial.println("BME280 connected");
  else
    Serial.println("BME280 not connected");


}


//Main---------------------------------------------------------------------------------------------------------------------------------------------------
void loop() {
  ArduinoOTA.handle();

  mqttClient.loop();

  getRainData();
  Ticker();

}
