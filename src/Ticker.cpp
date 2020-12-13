#include <main.h>





//Sekunden Ticker--------------------------------------------------------------------------------------------------------------------------------
void Ticker_1s(){


  #ifdef DEBUG_Prog
    digitalWrite(LED_BUILTIN , !digitalRead(LED_BUILTIN));
    Serial.print("ADC :");
    Serial.println(analogRead(A0));
  #endif
}

//10 Sekunden Ticker-------------------------------------------------------------------------------------------------------------------------------
void Ticker_10s(){
  #ifdef DEBUG_Prog
    Serial.println("Ticker 10s");
  #endif
}

//Minuten Ticker------------------------------------------------------------------------------------------------------------------------------------
void Ticker_1m(){


    getBMESensorData();


    Temp_Av_10m[Av_10m_Index] = fTemp;
    Hum_Av_10m[Av_10m_Index] = fHum;
    Pres_Av_10m[Av_10m_Index] = fPres;
    
    Av_10m_Index++;

    #ifdef DEBUG_Prog
        Serial.println("Ticker 1m");
        Serial.printf("%.2f C, %.1f hPa, %.1f %% Index : %d\n",Temp_Av_10m[Av_10m_Index - 1],Pres_Av_10m[Av_10m_Index - 1],Hum_Av_10m[Av_10m_Index - 1], Av_10m_Index);
        Serial.printf("Int16_t : %d\n",(int16_t)(Temp_Av_10m[Av_10m_Index - 1] * 100));
    #endif

    #ifndef SOLAR
      char buffer[32];
      sprintf(buffer , "%d" , unixTime);
      if(mqttClient.connected() == 0)
        mqtt_Connect();
      mqttClient.publish("Wetterstation/Unixtime" , buffer ,true);
    #endif
  //


}


//10 Minuten Ticker-----------------------------------------------------------------------------------------------------------------------------------
void Ticker_10m(){
  #ifdef DEBUG_Prog
    Serial.println("Ticker 10m");
  #endif

    float fTemp_Sum = 0.0;
    float fHum_Sum  = 0.0;
    float fPres_Sum = 0.0;

    for (uint8_t i = 0; i < Av_10m_Index; i++){
        fTemp_Sum   += Temp_Av_10m[i];
        fHum_Sum    += Hum_Av_10m[i];
        fPres_Sum   += Pres_Av_10m[i];
    }
    
    fTemp_Average_10m   = fTemp_Sum / Av_10m_Index;
    fHum_Average_10m    = fHum_Sum  / Av_10m_Index;
    fPres_Average_10m   = fPres_Sum / Av_10m_Index;

    Av_10m_Index = 0;


    Temp_Av = (int16_t)(fTemp_Average_10m * 100);
    Temp_Av_Count = readEEprom8(0x03);
    writeEEprom16((0x04 + (Temp_Av_Count*2)) , Temp_Av);
    Temp_Av_Count++;
    writeEEprom8(0x03 , Temp_Av_Count);
    
    #ifdef DEBUG
        Serial.printf("EEProm Coount : %d  , Temp : %d\n" ,readEEprom8(0x03) , (int16_t)readEEprom16(0x04 + (2*(Temp_Av_Count-1))));
    #endif

    timeClient.update();
    writeDatabase_VBatt();
    vDelay(1000);
    writeDatabase_10m();
    vDelay(1000);
}

//Stunden Ticker--------------------------------------------------------------------------------------------------------------------------------------
void Ticker_1h(){
  #ifdef DEBUG_Prog
    Serial.println("Ticker 1h");
  #endif
  //write rain data in db
  writeDatabase_1h();
  vDelay(1000);
  writeEEprom8(0x00 , 0); //EEprom Rain/hour = 0
}

//24 Stunden Ticker(02:00)---------------------------------------------------------------------------------------------------------------------------
void Ticker_24h(){
  #ifdef DEBUG_Prog
    Serial.println("Ticker 24h");
  #endif

  // write avg. Temp in db

  float fTemp_Sum = 0.0;

  for(uint16_t i = 0; i < Temp_Av_Count ; i++)
    fTemp_Sum += ((int16_t)readEEprom16(0x04 + (2*i)));

  fTemp_Sum /= 100;

  fTemp_Average = fTemp_Sum / Temp_Av_Count;

  writeDatabase_24h();
  vDelay(1000);
  writeEEprom8(0x01 , 0); //EEprom Rain/day = 0
  writeEEprom8(0x03 , 0); //EEProm Temp Count = 0

  vDelay(5000);
  ESP.restart();
}

//30 Tage Ticker------------------------------------------------------------------------------------------------------------------------------
void Ticker_30d(){
  ESP.restart();
}

//Ticker-------------------------------------------------------------------------------------------------------------------------------------
void Ticker(){
  uint32_t current_Millis = millis();

  if(current_Millis > millis_1S + INTERVALL_1S){
    millis_1S = current_Millis;
    Ticker_1s();

    
    unixTime = timeClient.getEpochTime();
    //Serial.printf("%d:%d:%d   Mod: %d Time : %d \n" , timeClient.getHours(), timeClient.getMinutes() , timeClient.getSeconds() , unixTime % 60 , time_2 - time_1);

    if(unixTime % 10 == 0)
      Ticker_10s();

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