
#include<Wire.h>                                              //Required for I2C protocol
#include<LiquidCrystal.h>                                     //Required for the LCD Display
#define ChipAddress 80                                        //Chip Address of the ROM retreived from the datasheet of 24LC128

const int rs = 12, en = 11, d0 = 9, d1 = 8, d2 = 7, d3 = 6, d4 = 5, d5 = 4, d6 = 3, d7 = 2;   //pins of the LCD initialised
LiquidCrystal lcd(rs, en, d0, d1, d2, d3, d4, d5, d6, d7);                //lcd object instantiated
void setup() {
  // put your setup code here, to run once:
  pinMode(A0, INPUT);                                         //pin A0 used for measuring Vout
  analogReference(EXTERNAL);                                  //used to change resolution of ADC to 4mV
  lcd.begin(16, 2);                                           //LCD started
  Wire.begin();                                               //I2C protocol started
}
float potval;
float vol;
void loop() {
  // put your main code here, to run repeatedly:

  potval = analogRead(A0);                                       //Voltage Read from pin A0
  //  potval = potval*4-2;
  potval = (int(((potval * (3.3 / 1.023))) / 4) * (4)) +2 ;
  //potval = (int)((potval*(3.3/1.023))/4)*4-2;
  vol = (potval) / (1000) ;
  if (vol < 0.248 || vol > 1.752)                              //If voltage is out of desired range (.25V-1.75V)
  {
    lcd.setCursor(0, 0);
    lcd.print("Voltage outside");
    lcd.setCursor(0, 1);
    lcd.print("range.");                                      //Voltage outside range printed on LCD
  }
  else
  {
    lcd.setCursor(0, 0);
    lcd.print("Voltage: ");
    lcd.print(vol, 3);
    lcd.print("V");                                             //Corresponding voltage printed on LCD
    delay(500);

    lcd.setCursor(0, 1);
    lcd.print("Power: ");
    vol = potval;
    float x, y;
    x = (int)readFrom(ChipAddress, (vol - 250) / 2);      //Power corresponding to voltage read from lookup table stored in EEPROM and displayed on LCD
    //  lcd.print(".");
    y = (float)readFrom(ChipAddress, (vol - 250) / 2 + 1) / 100;
    x = x + y;
    lcd.print(x, 2);
    lcd.print("dbm");
  }

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
