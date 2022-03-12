#include <Arduino.h>

#include "main.h"
#include "sensor.h"

//BME280 Auslesen------------------------------------------------------------------------------------------------------------------------
void getBMESensorData(float &temp , float &pres , float &hum){

  if(I2C_Error){
    digitalWrite(pinRelais , LOW);
    delay(1000);
    BME280.begin(0x76);
    delay(1000);

  }

  float fTemp_New = BME280.readTemperature();
  float fPres_New = BME280.readPressure() / 100;
  float fHum_New = BME280.readHumidity();

  // Sanity Check values to check is i2c is still working
  if(fTemp_New > 60.0 || fTemp_New <  -50.0){
    #ifdef DEBUG_Prog
      Serial.println("I2C Error !!!");
    #endif
    I2C_Error = 1;
    digitalWrite(pinRelais , HIGH);

  }else{
    temp = fTemp_New;
    pres = fPres_New;
    hum  = fHum_New;
  }

}

//Regenmenge ermitteln------------------------------------------------------------------------------------------------------------------------
void getRainData(){
  uint8_t pinRain_Status = digitalRead(pinRain);
  if(pinRain_Status == 0 && pinRain_Status_Old == 1){
    uint32_t rain_millis = millis()-rain_millis_old;
    rain_millis_old = millis();
    
    if(rain_millis > 200){
      rain_hour = readEEprom8(0x00);
      rain_day = readEEprom8(0x01);
      rain_day  ++;
      rain_hour ++;
      writeEEprom16(0x00 , ((uint16_t)rain_day << 8)|(uint16_t)rain_hour);
      #ifdef DEBUG_Prog
        Serial.printf("Rain/h : %d  Rain/d : %d\n",readEEprom8(0x00),readEEprom8(0x01));

      #endif

    }
  }
  pinRain_Status_Old = pinRain_Status;
}

//Batteriespannung ermitteln------------------------------------------------------------------------------------------------------------------------
float getVBatt(){
  uint32_t data;
  data = analogRead(A0);
  float volt = (float(data)/1023) * UREF * 2; 

  #ifdef DEBUG_Prog
    Serial.printf("Analog Read: %d , %f V\n",data , volt );
  #endif

  return volt;
}


//Windgeschwindigkeit auslesen------------------------------------------------------------------------------------------------------------------------
uint16_t getWindCounter(){

  Wire.beginTransmission(0x21);
  Wire.write(5);
  Wire.endTransmission();
  Wire.requestFrom(0x21 , 2);   

  uint16_t Wind_Counter_High = Wire.read();
  uint16_t Wind_Counter_Low  = Wire.read();

  Wire.beginTransmission(0x21);
  Wire.write(7);
  Wire.write(1);
  Wire.endTransmission();


  return (Wind_Counter_High << 8) + Wind_Counter_Low;
}

//Windrichtung ausslesen------------------------------------------------------------------------------------------------------------------------
uint8_t getWindDirection(){

  Wire.beginTransmission(0x21);
  Wire.write(12);
  Wire.endTransmission();
  Wire.requestFrom(0x21 , 1);   

  return Wire.read();
}