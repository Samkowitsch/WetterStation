#include <main.h>






//WiFi Einschalten-------------------------------------------------------------------------------------------------------------------------------
void WifiON(){
  Serial.println("Turn on Wifi");
  connectWifi();
  mqtt_Connect();
}

//WiFi Ausschalten-------------------------------------------------------------------------------------------------------------------------------
void WifiOFF(){
  vDelay(200);
  //mqttClient.publish("Wetterstation/Connection" , "Disconnected");
  vDelay(200);
  mqttClient.disconnect();
  vDelay(200);
  Serial.println("Turn off Wifi");
  WiFi.mode(WIFI_OFF);
  WiFi.forceSleepBegin();
}

//Status der Wifi Verbidnung------------------------------------------------------------------------------------------------------------------------
byte checkWlanConnection(){
  byte Wlan_Status = WiFi.isConnected();

  return Wlan_Status;
}

//Wifi verbinden------------------------------------------------------------------------------------------------------------------------
void connectWifi(){

  #ifdef DEBUG_Prog
    Serial.println("Turning ON Wifi .");
  #endif

  WiFi.mode(WIFI_STA);

  vDelay(500);

  WiFi.begin(SSID_2,SSID_PASSWORD);

  Serial.printf("Connecting to Wlan %s :" , SSID_2);
  //while (wifiMulti.run() != WL_CONNECTED){
  while(WiFi.status() != WL_CONNECTED){
      vDelay(200);

      #ifdef DEBUG_Prog
        Serial.print(".");
      #endif
  }

  #ifdef DEBUG_Prog
    Serial.println("\n");
    Serial.print("Connected to :");
    Serial.print(WiFi.SSID());
    Serial.print(" Ip-Adress :");
    Serial.println(WiFi.localIP());
  #endif
}

//Arduino OTA------------------------------------------------------------------------------------------------------------------------
void setupOTA(){
  ArduinoOTA.setHostname("WetterStation");
  ArduinoOTA.setPassword("WetterStation");

  ArduinoOTA.onStart([]() {
    Serial.println("ArduinoOTA Start");
  });

  ArduinoOTA.onEnd([]() {
    Serial.println("ArduinoOTA End");
  });

  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total){
    Serial.printf("Progress : %u%%\r", (progress/(total/100)));
  });

  ArduinoOTA.onError([](ota_error_t error){
    Serial.printf("Error[%u]:", error);
    if(error == OTA_AUTH_ERROR) 
      Serial.println("Auth. Failed");
    else if(error == OTA_BEGIN_ERROR)
      Serial.println("Begin Failed");
    else if(error == OTA_CONNECT_ERROR)
      Serial.println("Connect Failed");
    else if(error == OTA_RECEIVE_ERROR)
      Serial.println("Receive Failed");
    else if(error == OTA_END_ERROR)
      Serial.println("End failed");
  });

  ArduinoOTA.begin();
  Serial.println("ArduinoOTA ready");
}
//Mqtt Connect------------------------------------------------------------------

uint8_t mqtt_Connect(){

 // mqttClient.setServer(mqtt_Broker_IP , 1883);



  while(!mqttClient.connected()){

    if(mqttClient.connect(mqtt_ClientId)){
      //mqttClient.publish("WeatherStat/Outdoor/Mqtt" ,  "Connected" );
    }else{
      // Serial.print("\nConnection failed with state:");
      // Serial.println(mqttClient.state());
    }
    

  }
return 1;
}


// Mqtt Callback Function-------------------------------------------------------

void mqtt_Callback(char* topic , byte* payload , int length){



};