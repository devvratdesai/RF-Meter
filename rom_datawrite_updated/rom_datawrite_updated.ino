
#include <Wire.h> //Required for I2C protocol

#define ChipAddress 80                                        //Chip Address of the ROM retreived from the datasheet of 24LC128

         
const float power[] PROGMEM={0.97,1.11,1.24,1.38,1.51,1.64,1.77,1.89,2.02,2.14,2.26,2.38,2.49,2.61,2.72,2.84,2.95,3.06,3.17,3.27,3.38,3.49,3.59,3.69,3.79,3.89,3.99,4.09,4.18,4.28,4.37,4.47,4.56,4.65,4.74,4.83,4.92,5.01,5.09,5.18,5.27,5.35,5.43,5.52,5.6,5.68,5.76,5.84,5.92,6,6.07,6.15,6.23,6.3,6.38,6.45,6.53,6.6,6.67,6.74,6.81,6.88,6.95,7.02,7.09,7.16,7.23,7.3,7.36,7.43,7.5,7.56,7.63,7.69,7.75,7.82,7.88,7.94,8.01,8.07,8.13,8.19,8.25,8.31,8.37,8.43,8.49,8.54,8.6,8.66,8.72,8.77,8.83,8.89,8.94,9,9.05,9.11,9.16,9.21,9.27,9.32,9.37,9.43,9.48,9.53,9.58,9.63,9.69,9.74,9.79,9.84,9.89,9.94,9.99,10.04,10.08,10.13,10.18,10.23,10.28,10.32,10.37,10.42,10.47,10.51,10.56,10.6,10.65,10.69,10.74,10.79,10.83,10.87,10.92,10.96,11.01,11.05,11.09,11.14,11.18,11.22,11.27,11.31,11.35,11.39,11.43,11.48,11.52,11.56,11.6,11.64,11.68,11.72,11.76,11.8,11.84,11.88,11.92,11.96,12,12.04,12.08,12.11,12.15,12.19,12.23,12.27,12.3,12.34,12.38,12.42,12.45,12.49,12.53,12.56,12.6,12.64,12.67,12.71,12.75,12.78,12.82,12.85,12.89,12.92,12.96,12.99,13.03,13.06,13.1,13.13,13.17,13.2,13.23,13.27,13.3,13.33,13.37,13.4,13.43,13.47,13.5,13.53,13.57,13.6,13.63,13.66,13.69,13.73,13.76,13.79,13.82,13.85,13.89,13.92,13.95,13.98,14.01,14.04,14.07,14.1,14.13,14.16,14.19,14.22,14.25,14.28,14.31,14.34,14.37,14.4,14.43,14.46,14.49,14.52,14.55,14.58,14.61,14.64,14.67,14.69,14.72,14.75,14.78,14.81,14.84,14.86,14.89,14.92,14.95,14.98,15,15.03,15.06,15.09,15.11,15.14,15.17,15.2,15.22,15.25,15.28,15.3,15.33,15.36,15.38,15.41,15.43,15.46,15.49,15.51,15.54,15.57,15.59,15.62,15.64,15.67,15.69,15.72,15.74,15.77,15.8,15.82,15.85,15.87,15.9,15.92,15.95,15.97,15.99,16.02,16.04,16.07,16.09,16.12,16.14,16.17,16.19,16.21,16.24,16.26,16.29,16.31,16.33,16.36,16.38,16.4,16.43,16.45,16.47,16.5,16.52,16.54,16.57,16.59,16.61,16.64,16.66,16.68,16.7,16.73,16.75,16.77,16.79,16.82,16.84,16.86,16.88,16.91,16.93,16.95,16.97,16.99,17.02,17.04,17.06,17.08,17.1,17.13,17.15,17.17,17.19,17.21,17.23,17.25,17.28,17.3,17.32,17.34,17.36,17.38,17.4,17.42,17.44,17.46,17.49,17.51,17.53,17.55,17.57,17.59,17.61,17.63,17.65,17.67,17.69,17.71,17.73,17.75,17.77,17.79,17.81,17.83,17.85,17.87};

void setup() {
  // put your setup code here, to run once:
  Wire.begin(); //I2C protocol started
  int j,i;
  
  for(int i=0;i<=752;i+=2)
  { j=i/2;
    writeTo(ChipAddress,i,(int)power[j]);
    writeTo(ChipAddress,i+1, 100*(power[j]-(int)power[j]+0.005));
  }  
  
}

void loop() {
  // put your main code here, to run repeatedly:
}


void writeTo(int chAddress, unsigned int ceAddress, byte wData)
{
  Wire.beginTransmission(chAddress);    //Start sequence followed by Chip Address sent
  Wire.write(ceAddress >> 8);         //Upper Byte of the Cell address to write sent first
  Wire.write(ceAddress & 0xFF);       //Lower Byte of the Cell address sent 
  Wire.write(wData);                //Data to be written sent
  Wire.endTransmission();         //Transmission ended with a stop sequence

  delay(1);
}
byte readFrom(int chAddress, unsigned int ceAddress)
{
  Wire.beginTransmission(chAddress);                            //Start sequence followed by Chip Address sent
  Wire.write(ceAddress >> 8);                                   //Upper Byte of the Cell address to read from sent first
  Wire.write(ceAddress & 0xFF);                                 //Lower Byte of the Cell address sent 
  Wire.endTransmission();                                       //Transmission ended with a stop sequence

  byte rData = 0;
  Wire.requestFrom(chAddress, 1);                               //One byte of data read from Cell address

  if (Wire.available())                       
    rData = Wire.read();                                        //Byte stored in rData
    
  return rData; 
}
