#include <main.h>

#define SMOOTHING_FACTOR 10.0

bool  avg_init = false;
float avg_temp;
float avg_pres;
float avg_hum;
float avg_vbat;

// Analog Filer (exponential Filter)---------------------------------------------------------------------------------
float exp_filter(float value , float value_old , float smoothing_factor){
	if(smoothing_factor > 100)
		smoothing_factor = 100;
	
	float value_smoothed =((smoothing_factor * value)+ ((100 - smoothing_factor)*value_old))/100; 	
return value_smoothed;
}


//Sekunden Ticker--------------------------------------------------------------------------------------------------------------------------------
void Ticker_1s(){

  float temp , pres , hum , vbat;

  getBMESensorData(temp , pres ,hum);
  vbat = getVBatt();

  //direction = getWindDirection() * 22.5;


  if(mqttClient.connected() == 0)
    mqtt_Connect();


  char payload[128];
  sprintf(payload , "{ \"temp\":%.1f, \"pres\":%.1f, \"hum\":%.1f, \"ts\":%d}", temp , pres , hum , unixTime );
  mqttClient.publish("WeatherStat/Outdoor/Act" ,payload);

  if(avg_init == true){
    exp_filter(temp , avg_temp , SMOOTHING_FACTOR);
    exp_filter(pres , avg_pres , SMOOTHING_FACTOR);
    exp_filter(hum  , avg_hum  , SMOOTHING_FACTOR);
    exp_filter(vbat , avg_vbat , SMOOTHING_FACTOR);
  }else{
    avg_temp = temp;
    avg_pres = pres;
    avg_hum  = hum;
    avg_vbat = vbat;
    avg_init = true;
  }

}

//10 Sekunden Ticker-------------------------------------------------------------------------------------------------------------------------------
void Ticker_10s(){


}

//Minuten Ticker------------------------------------------------------------------------------------------------------------------------------------
void Ticker_1m(){



}


//10 Minuten Ticker-----------------------------------------------------------------------------------------------------------------------------------
void Ticker_10m(){

 if(mqttClient.connected() == 0)
    mqtt_Connect();

  float speed = float(getWindCounter()) / 250;
  float direction = getWindDirection() * 22.5;

  char payload[256];
  sprintf(payload , "{\"temp\":%.1f, \"pres\":%.1f, \"hum\":%.1f, \"speed\": %.1f, \"dir\": %.1f, \"batt\": %.2f, \"ts\":%d}", avg_temp , avg_pres , avg_hum , speed , direction , avg_vbat , unixTime );
  mqttClient.publish("WeatherStat/Outdoor/Avg" , payload);

  Serial.println("10 min Ticker ! ");
  timeClient.update();

}

//Stunden Ticker--------------------------------------------------------------------------------------------------------------------------------------
void Ticker_1h(){

}

//24 Stunden Ticker(02:00)---------------------------------------------------------------------------------------------------------------------------
void Ticker_24h(){

  ESP.restart();
}

//30 Tage Ticker------------------------------------------------------------------------------------------------------------------------------
void Ticker_30d(){
  //ESP.restart();
}


//Ticker-------------------------------------------------------------------------------------------------------------------------------------
void Ticker(){
  uint32_t current_Millis = millis();

  if(current_Millis > millis_1S + INTERVALL_1S){
    millis_1S = current_Millis;
    Ticker_1s();

    
    unixTime = timeClient.getEpochTime();
    //Serial.printf("%d:%d:%d   Mod: %d Time : %d \n" , timeClient.getHours(), timeClient.getMinutes() , timeClient.getSeconds() , unixTime % 60 , time_2 - time_1);

    if(unixTime % 10 == 0){
      Ticker_10s();
    }

    if(unixTime % 60 == 0)
      Ticker_1m();

    if((unixTime + 30) % 600 == 0){
      if(checkWlanConnection() != 1)
        WifiON();
    }

    if(unixTime % 600 == 0)
      Ticker_10m();

    if(unixTime % 3600 == 0)
      Ticker_1h();

    if(unixTime % 86400 == 0)
      Ticker_24h();

    #ifdef SOLAR
      if((unixTime - 30) % 600 == 0)
        WifiOFF();
    #endif
      
  }
}