#include <main.h>


//BVatteriespannung Database schreiben------------------------------------------------------------------------------------------------------------------------
void writeDatabase_VBatt(){

  char buffer[256];
  float Spannung = getVBatt();


  char Insert_Sql_2[] = "INSERT INTO Wetterstation.V_Batt (Spannung , UnixTime) VALUES (%.2f , %d)";
  if(conn.connect(mySqlServer , 3306 , mySqlUser , mySqlPw)){

    vDelay(100);
    MySQL_Cursor *cur_mem = new MySQL_Cursor(&conn);

    sprintf(buffer , Insert_Sql_2 , Spannung , unixTime );
    cur_mem->execute(buffer);

    delete cur_mem;
  }
  conn.close();

  vDelay(500);
  if(mqttClient.connected() == 0)
    mqtt_Connect();
  mqttClient.publish("Wetterstation/Write_DB" , "V_Batt");
  //mqttClient.publish("Wetterstation/V_Batt_Insert" , buffer , true);
  sprintf(buffer , "%.2f", Spannung);
  mqttClient.publish("Wetterstation/V_Batt" , buffer);
  vDelay(500);

}

//10m Database schreiben------------------------------------------------------------------------------------------------------------------------
void writeDatabase_10m(){

  Serial.println("Write Database_10m");
  char buffer[256];

  char Insert_Sql[] = "INSERT INTO Wetterstation.Database_10m (Temperatur, Luftdruck, Luftfeuchte , Windgeschwindigkeit , Windrichtung , UnixTime) VALUES (%.1f, %.1f , %.1f , %.1f , %.1f , %d)";
  //INSERT INTO `Database_10m` (`id`, `Temperatur`, `Luftdruck`, `Luftfeuchte`, `Windgeschwindigkeit`, `Windrichtung`, `Zeitstempel`, `Datum`) VALUES (NULL, '', '', '', '', '', NULL, current_timestamp())
  //sprintf(cunixTime , "%d" , unixTime);


  fWind_Speed = float(getWindCounter()) / 250;
  fWind_Dir = getWindDirection() * 22.5;

  /*fWind_Dir = 0.0;
  fWind_Speed = 0.0;
  fTemp = 0.0;
  fHum = 0.0;
  fPres= 0.0;
  	*/

  if(conn.connect(mySqlServer , 3306 , mySqlUser , mySqlPw)){
    Serial.println("Database_10m Connected");
    vDelay(100);
    MySQL_Cursor *cur_mem = new MySQL_Cursor(&conn);

    sprintf(buffer , Insert_Sql , fTemp_Average_10m , fPres_Average_10m , fHum_Average_10m , fWind_Speed , fWind_Dir , unixTime );
    //Serial.println(buffer);
    cur_mem->execute(buffer);

    delete cur_mem;
  }

  conn.close();

  vDelay(500);
  if(mqttClient.connected() == 0)
    mqtt_Connect();
  mqttClient.publish("Wetterstation/Write_DB" , "Database 10m");
  //mqttClient.publish("Wetterstation/Database_10m_Insert" , buffer , true);
  sprintf(buffer , "{ \"Temp\":%.1f, \"Pres\":%.1f, \"Hum\":%.1f, \"Speed\":%.1f, \"Dir\":%.1f , \"Time\":%d}", fTemp_Average_10m , fPres_Average_10m , fHum_Average_10m , fWind_Speed , fWind_Dir , unixTime );
  mqttClient.publish("Wetterstation/Values" , buffer , true);
  vDelay(500);


}

//1h Database schreiben------------------------------------------------------------------------------------------------------------------------
void writeDatabase_1h(){
  char buffer[256];
  char Insert_Sql[] = "INSERT INTO Wetterstation.Database_1h (Niederschlag, UnixTime) VALUES (%.2f , %d)";

  if(conn.connect(mySqlServer , 3306 , mySqlUser , mySqlPw)){
    vDelay(100);
    MySQL_Cursor *cur_mem = new MySQL_Cursor(&conn);

    sprintf(buffer , Insert_Sql , (readEEprom8(0x00) * mm_TickRain)  , unixTime );
    cur_mem->execute(buffer);

    delete cur_mem;
  }
  conn.close();

  vDelay(500);
  if(mqttClient.connected() == 0)
    mqtt_Connect();
  mqttClient.publish("Wetterstation/Write_DB" , "Database 1h");
  //mqttClient.publish("Wetterstation/Database_1h_Insert" , buffer , true);
  vDelay(500);

}

//24h Database schreiben------------------------------------------------------------------------------------------------------------------------
void writeDatabase_24h(){
  char buffer[256];
  char Insert_Sql[] = "INSERT INTO Wetterstation.Database_24h (Temperatur , Niederschlag , UnixTime) VALUES (%.1f , %.2f , %d)";

  if(conn.connect(mySqlServer , 3306 , mySqlUser , mySqlPw)){
    vDelay(100);
    MySQL_Cursor *cur_mem = new MySQL_Cursor(&conn);

    sprintf(buffer , Insert_Sql , fTemp_Average , (readEEprom8(0x01) * mm_TickRain)  , unixTime );
    cur_mem->execute(buffer);

    delete cur_mem;
  }
  conn.close();

  vDelay(500);
  if(mqttClient.connected() == 0)
    mqtt_Connect();
  mqttClient.publish("Wetterstation/Write_DB" , "Database 24h");
  //mqttClient.publish("Wetterstation/Database_24h_Insert" , buffer , true);
  vDelay(500);

}