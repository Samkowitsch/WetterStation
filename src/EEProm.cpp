#include <main.h>




uint8_t readEEprom8(uint16_t reg){
  uint8_t Addres = 0x50;

  if(reg > 0xFF)
    Addres++;

  Wire.beginTransmission(Addres);
  Wire.write(uint8_t(reg & 0xFF));
  Wire.endTransmission();
  Wire.requestFrom(Addres , 1);   

return Wire.read();
}

uint16_t readEEprom16(uint16_t reg){
  uint8_t Addres = 0x50;

  if(reg > 0xFF)
    Addres++;

  Wire.beginTransmission(Addres);
  Wire.write(uint8_t(reg & 0xFF));
  Wire.endTransmission();
  Wire.requestFrom(Addres , 2);   

  uint16_t dataHigh = Wire.read() << 8;
  uint16_t dataLow  = Wire.read();

return dataHigh | dataLow;
}

void writeEEprom8(uint16_t reg , uint8_t value){
  uint8_t Addres = 0x50;

  if(reg > 0xFF)
    Addres++;

  Wire.beginTransmission(Addres);
  Wire.write(uint8_t(reg & 0xFF));
  Wire.write(value);
  Wire.endTransmission();
  delay(5);
}

void writeEEprom16(uint16_t reg , uint16_t value){
  uint8_t dataHigh = value >> 8;
  uint8_t dataLow  = value & 0xFF;
  uint8_t Addres = 0x50;

  if(reg > 0xFF)
    Addres++;

  Wire.beginTransmission(Addres);
  Wire.write(uint8_t(reg & 0xFF));
  Wire.write(dataHigh);
  Wire.write(dataLow);
  Wire.endTransmission();
  delay(5);
}
