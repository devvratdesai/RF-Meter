#include <avr/io.h>
#include <util/delay.h>
#define F_CPU 8000000UL                                            //Required for I2C protocol
#include<LiquidCrystal.h>                                     //Required for the LCD Display
#define ChipAddress 80                                        //Chip Address of the ROM retreived from the datasheet of 24LC128

float attenuator = 20;

int dip_switch(int a,int b)
{
  int num =  (((a&0x01)<< 1) | (b&0x01));   
  return num;
}

void ADC_Init()
{
  DDRA=0x0;     /* Make ADC port as input */
  ADCSRA = 0x87;      /* Enable ADC, fr/128  */
  ADMUX = 0x00;     /* Vref: Avcc, ADC channel: 0 */
}

  
const int rs = PIN_PC0, en = PIN_PC1, d0 = PIN_PD0, d1 = PIN_PD1, d2 = PIN_PD2, d3 = PIN_PD3, d4 = PIN_PD4, d5 = PIN_PD5, d6 = PIN_PD6, d7 = PIN_PD7;   //pins of the LCD initialised
LiquidCrystal lcd(rs, en, d0, d1, d2, d3, d4, d5, d6, d7);                //lcd object instantiated
void setup() {
  // put your setup code here, to run once:
  pinMode(PIN_PA0, INPUT);                                         //pin A0 used for measuring Vout
 // analogReference(EXTERNAL);                                  //used to change resolution of ADC to 4mV
  lcd.begin(16, 2);                                           //LCD started
  ADC_Init();                                     //I2C protocol started
}

const float power[4][376] PROGMEM={{0.97,1.11,1.24,1.38,1.51,1.64,1.77,1.89,2.02,2.14,2.26,2.38,2.49,2.61,2.72,2.84,2.95,3.06,3.17,3.27,3.38,3.49,3.59,3.69,3.79,3.89,3.99,4.09,4.18,4.28,4.37,4.47,4.56,4.65,4.74,4.83,4.92,5.01,5.09,5.18,5.27,5.35,5.43,5.52,5.6,5.68,5.76,5.84,5.92,6,6.07,6.15,6.23,6.3,6.38,6.45,6.53,6.6,6.67,6.74,6.81,6.88,6.95,7.02,7.09,7.16,7.23,7.3,7.36,7.43,7.5,7.56,7.63,7.69,7.75,7.82,7.88,7.94,8.01,8.07,8.13,8.19,8.25,8.31,8.37,8.43,8.49,8.54,8.6,8.66,8.72,8.77,8.83,8.89,8.94,9,9.05,9.11,9.16,9.21,9.27,9.32,9.37,9.43,9.48,9.53,9.58,9.63,9.69,9.74,9.79,9.84,9.89,9.94,9.99,10.04,10.08,10.13,10.18,10.23,10.28,10.32,10.37,10.42,10.47,10.51,10.56,10.6,10.65,10.69,10.74,10.79,10.83,10.87,10.92,10.96,11.01,11.05,11.09,11.14,11.18,11.22,11.27,11.31,11.35,11.39,11.43,11.48,11.52,11.56,11.6,11.64,11.68,11.72,11.76,11.8,11.84,11.88,11.92,11.96,12,12.04,12.08,12.11,12.15,12.19,12.23,12.27,12.3,12.34,12.38,12.42,12.45,12.49,12.53,12.56,12.6,12.64,12.67,12.71,12.75,12.78,12.82,12.85,12.89,12.92,12.96,12.99,13.03,13.06,13.1,13.13,13.17,13.2,13.23,13.27,13.3,13.33,13.37,13.4,13.43,13.47,13.5,13.53,13.57,13.6,13.63,13.66,13.69,13.73,13.76,13.79,13.82,13.85,13.89,13.92,13.95,13.98,14.01,14.04,14.07,14.1,14.13,14.16,14.19,14.22,14.25,14.28,14.31,14.34,14.37,14.4,14.43,14.46,14.49,14.52,14.55,14.58,14.61,14.64,14.67,14.69,14.72,14.75,14.78,14.81,14.84,14.86,14.89,14.92,14.95,14.98,15,15.03,15.06,15.09,15.11,15.14,15.17,15.2,15.22,15.25,15.28,15.3,15.33,15.36,15.38,15.41,15.43,15.46,15.49,15.51,15.54,15.57,15.59,15.62,15.64,15.67,15.69,15.72,15.74,15.77,15.8,15.82,15.85,15.87,15.9,15.92,15.95,15.97,15.99,16.02,16.04,16.07,16.09,16.12,16.14,16.17,16.19,16.21,16.24,16.26,16.29,16.31,16.33,16.36,16.38,16.4,16.43,16.45,16.47,16.5,16.52,16.54,16.57,16.59,16.61,16.64,16.66,16.68,16.7,16.73,16.75,16.77,16.79,16.82,16.84,16.86,16.88,16.91,16.93,16.95,16.97,16.99,17.02,17.04,17.06,17.08,17.1,17.13,17.15,17.17,17.19,17.21,17.23,17.25,17.28,17.3,17.32,17.34,17.36,17.38,17.4,17.42,17.44,17.46,17.49,17.51,17.53,17.55,17.57,17.59,17.61,17.63,17.65,17.67,17.69,17.71,17.73,17.75,17.77,17.79,17.81,17.83,17.85,17.87},
                                 {1.43,1.56,1.70,1.83,1.97,2.10,2.22,2.35,2.47,2.60,2.72,2.83,2.95,3.07,3.18,3.30,3.41,3.52,3.62,3.73,3.84,3.94,4.05,4.15,4.25,4.35,4.45,4.55,4.64,4.74,4.83,4.93,5.02,5.11,5.20,5.29,5.38,5.47,5.55,5.64,5.72,5.81,5.89,5.97,6.06,6.14,6.22,6.30,6.38,6.45,6.53,6.61,6.69,6.76,6.84,6.91,6.98,7.06,7.13,7.20,7.27,7.34,7.41,7.48,7.55,7.62,7.69,7.75,7.82,7.89,7.95,8.02,8.08,8.15,8.21,8.28,8.34,8.40,8.46,8.52,8.59,8.65,8.71,8.77,8.83,8.88,8.94,9.00,9.06,9.12,9.17,9.23,9.29,9.34,9.40,9.45,9.51,9.56,9.62,9.67,9.73,9.78,9.83,9.89,9.94,9.99,10.04,10.09,10.14,10.19,10.24,10.30,10.34,10.39,10.44,10.49,10.54,10.59,10.64,10.69,10.73,10.78,10.83,10.88,10.92,10.97,11.02,11.06,11.11,11.15,11.20,11.24,11.29,11.33,11.38,11.42,11.46,11.51,11.55,11.59,11.64,11.68,11.72,11.77,11.81,11.85,11.89,11.93,11.97,12.02,12.06,12.10,12.14,12.18,12.22,12.26,12.30,12.34,12.38,12.42,12.46,12.49,12.53,12.57,12.61,12.65,12.69,12.72,12.76,12.80,12.84,12.87,12.91,12.95,12.99,13.02,13.06,13.10,13.13,13.17,13.20,13.24,13.27,13.31,13.35,13.38,13.42,13.45,13.49,13.52,13.55,13.59,13.62,13.66,13.69,13.72,13.76,13.79,13.83,13.86,13.89,13.92,13.96,13.99,14.02,14.06,14.09,14.12,14.15,14.18,14.22,14.25,14.28,14.31,14.34,14.37,14.41,14.44,14.47,14.50,14.53,14.56,14.59,14.62,14.65,14.68,14.71,14.74,14.77,14.80,14.83,14.86,14.89,14.92,14.95,14.98,15.01,15.04,15.07,15.09,15.12,15.15,15.18,15.21,15.24,15.27,15.29,15.32,15.35,15.38,15.41,15.43,15.46,15.49,15.52,15.54,15.57,15.60,15.63,15.65,15.68,15.71,15.73,15.76,15.79,15.81,15.84,15.87,15.89,15.92,15.94,15.97,16.00,16.02,16.05,16.07,16.10,16.13,16.15,16.18,16.20,16.23,16.25,16.28,16.30,16.33,16.35,16.38,16.40,16.43,16.45,16.48,16.50,16.53,16.55,16.57,16.60,16.62,16.65,16.67,16.70,16.72,16.74,16.77,16.79,16.81,16.84,16.86,16.88,16.91,16.93,16.95,16.98,17.00,17.02,17.05,17.07,17.09,17.12,17.14,17.16,17.18,17.21,17.23,17.25,17.27,17.30,17.32,17.34,17.36,17.39,17.41,17.43,17.45,17.47,17.50,17.52,17.54,17.56,17.58,17.60,17.63,17.65,17.67,17.69,17.71,17.73,17.75,17.78,17.80,17.82,17.84,17.86,17.88,17.90,17.92,17.94,17.96,17.98,18.01,18.03,18.05,18.07,18.09,18.11,18.13,18.15,18.17,18.19,18.21,18.23,18.25,18.27,18.29,18.31,18.33},
                                 {0.56,0.69,0.83,0.96,1.09,1.22,1.35,1.48,1.60,1.72,1.84,1.96,2.08,2.20,2.31,2.42,2.53,2.64,2.75,2.86,2.97,3.07,3.17,3.28,3.38,3.48,3.58,3.67,3.77,3.87,3.96,4.05,4.15,4.24,4.33,4.42,4.51,4.59,4.68,4.77,4.85,4.94,5.02,5.10,5.18,5.27,5.35,5.43,5.50,5.58,5.66,5.74,5.81,5.89,5.96,6.04,6.11,6.18,6.26,6.33,6.40,6.47,6.54,6.61,6.68,6.75,6.82,6.88,6.95,7.02,7.08,7.15,7.21,7.28,7.34,7.40,7.47,7.53,7.59,7.65,7.71,7.77,7.83,7.89,7.95,8.01,8.07,8.13,8.19,8.25,8.30,8.36,8.42,8.47,8.53,8.58,8.64,8.69,8.75,8.80,8.85,8.91,8.96,9.01,9.07,9.12,9.17,9.22,9.27,9.32,9.37,9.42,9.47,9.52,9.57,9.62,9.67,9.72,9.77,9.82,9.86,9.91,9.96,10.00,10.05,10.10,10.14,10.19,10.24,10.28,10.33,10.37,10.42,10.46,10.50,10.55,10.59,10.64,10.68,10.72,10.77,10.81,10.85,10.89,10.94,10.98,11.02,11.06,11.10,11.14,11.18,11.23,11.27,11.31,11.35,11.39,11.43,11.47,11.51,11.55,11.58,11.62,11.66,11.70,11.74,11.78,11.82,11.85,11.89,11.93,11.97,12.00,12.04,12.08,12.11,12.15,12.19,12.22,12.26,12.30,12.33,12.37,12.40,12.44,12.47,12.51,12.54,12.58,12.61,12.65,12.68,12.72,12.75,12.79,12.82,12.85,12.89,12.92,12.95,12.99,13.02,13.05,13.09,13.12,13.15,13.18,13.22,13.25,13.28,13.31,13.34,13.38,13.41,13.44,13.47,13.50,13.53,13.57,13.60,13.63,13.66,13.69,13.72,13.75,13.78,13.81,13.84,13.87,13.90,13.93,13.96,13.99,14.02,14.05,14.08,14.11,14.14,14.17,14.19,14.22,14.25,14.28,14.31,14.34,14.37,14.39,14.42,14.45,14.48,14.51,14.53,14.56,14.59,14.62,14.65,14.67,14.70,14.73,14.75,14.78,14.81,14.84,14.86,14.89,14.92,14.94,14.97,14.99,15.02,15.05,15.07,15.10,15.13,15.15,15.18,15.20,15.23,15.25,15.28,15.31,15.33,15.36,15.38,15.41,15.43,15.46,15.48,15.51,15.53,15.56,15.58,15.61,15.63,15.65,15.68,15.70,15.73,15.75,15.78,15.80,15.82,15.85,15.87,15.90,15.92,15.94,15.97,15.99,16.01,16.04,16.06,16.08,16.11,16.13,16.15,16.18,16.20,16.22,16.24,16.27,16.29,16.31,16.34,16.36,16.38,16.40,16.43,16.45,16.47,16.49,16.51,16.54,16.56,16.58,16.60,16.62,16.65,16.67,16.69,16.71,16.73,16.75,16.78,16.80,16.82,16.84,16.86,16.88,16.90,16.92,16.95,16.97,16.99,17.01,17.03,17.05,17.07,17.09,17.11,17.13,17.15,17.17,17.20,17.22,17.24,17.26,17.28,17.30,17.32,17.34,17.36,17.38,17.40,17.42,17.44,17.46},
                                 {1.94,2.08,2.21,2.35,2.48,2.61,2.73,2.86,2.98,3.11,3.23,3.35,3.46,3.58,3.69,3.81,3.92,4.03,4.14,4.24,4.35,4.45,4.56,4.66,4.76,4.86,4.96,5.06,5.15,5.25,5.34,5.44,5.53,5.62,5.71,5.80,5.89,5.98,6.06,6.15,6.24,6.32,6.40,6.49,6.57,6.65,6.73,6.81,6.89,6.97,7.04,7.12,7.20,7.27,7.35,7.42,7.49,7.57,7.64,7.71,7.78,7.85,7.92,7.99,8.06,8.13,8.20,8.27,8.33,8.40,8.46,8.53,8.60,8.66,8.72,8.79,8.85,8.91,8.97,9.04,9.10,9.16,9.22,9.28,9.34,9.40,9.46,9.51,9.57,9.63,9.69,9.74,9.80,9.86,9.91,9.97,10.02,10.08,10.13,10.18,10.24,10.29,10.34,10.40,10.45,10.50,10.55,10.60,10.66,10.71,10.76,10.81,10.86,10.91,10.96,11.00,11.05,11.10,11.15,11.20,11.25,11.29,11.34,11.39,11.43,11.48,11.53,11.57,11.62,11.66,11.71,11.75,11.80,11.84,11.89,11.93,11.98,12.02,12.06,12.11,12.15,12.19,12.23,12.28,12.32,12.36,12.40,12.44,12.49,12.53,12.57,12.61,12.65,12.69,12.73,12.77,12.81,12.85,12.89,12.93,12.97,13.01,13.04,13.08,13.12,13.16,13.20,13.24,13.27,13.31,13.35,13.39,13.42,13.46,13.50,13.53,13.57,13.61,13.64,13.68,13.71,13.75,13.79,13.82,13.86,13.89,13.93,13.96,14.00,14.03,14.07,14.10,14.13,14.17,14.20,14.24,14.27,14.30,14.34,14.37,14.40,14.44,14.47,14.50,14.53,14.57,14.60,14.63,14.66,14.70,14.73,14.76,14.79,14.82,14.85,14.89,14.92,14.95,14.98,15.01,15.04,15.07,15.10,15.13,15.16,15.19,15.22,15.25,15.28,15.31,15.34,15.37,15.40,15.43,15.46,15.49,15.52,15.55,15.58,15.61,15.64,15.66,15.69,15.72,15.75,15.78,15.81,15.83,15.86,15.89,15.92,15.95,15.97,16.00,16.03,16.06,16.08,16.11,16.14,16.16,16.19,16.22,16.24,16.27,16.30,16.32,16.35,16.38,16.40,16.43,16.46,16.48,16.51,16.53,16.56,16.59,16.61,16.64,16.66,16.69,16.71,16.74,16.76,16.79,16.81,16.84,16.86,16.89,16.91,16.94,16.96,16.99,17.01,17.04,17.06,17.09,17.11,17.13,17.16,17.18,17.21,17.23,17.25,17.28,17.30,17.33,17.35,17.37,17.40,17.42,17.44,17.47,17.49,17.51,17.54,17.56,17.58,17.60,17.63,17.65,17.67,17.70,17.72,17.74,17.76,17.79,17.81,17.83,17.85,17.88,17.90,17.92,17.94,17.96,17.99,18.01,18.03,18.05,18.07,18.09,18.12,18.14,18.16,18.18,18.20,18.22,18.24,18.27,18.29,18.31,18.33,18.35,18.37,18.39,18.41,18.43,18.45,18.48,18.50,18.52,18.54,18.56,18.58,18.60,18.62,18.64,18.66,18.68,18.70,18.72,18.74,18.76,18.78,18.80,18.82,18.84}};
// Row 0 - 50 ohms, Row 1 - 45 ohms, Row 2 - 55 ohms, Row 3 - 40 ohms  
                             
float potval;
float vol;
void loop() {
  int num = dip_switch(digitalRead(PIN_PB0),digitalRead(PIN_PB1));
  // put your main code here, to run repeatedly:
   ADC_Init();  
  lcd.print(num);
  delay(2000);
  potval = analogRead(A0);                                       //Voltage Read from pin A0
  potval = (int(((potval * (3.3 / 1.023))-1.6129) / 4) * (4)) +2 ; 
  vol = (potval) / (1000) ;                                     //converting voltage read into V
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
    lcd.print("V");                                             //Corresponding voltage upto 3 decimal points printed on LCD
    _delay_ms(500);

   
    vol = potval;
    float x;
    int l;
    l=(vol-250)/4;
    x=pgm_read_float_near( power[num] + l );
    
    // now x gives the power in Dbm
    x=x+attenuator;
    float k = x/10;
    float p = pow(10,k);                                      // formula for converting power in Dbm to uW , where in Power in uW is stored in variable p
    while(1)
    { 
      lcd.setCursor(0, 1);
      lcd.print("Power:");
      lcd.print(p,2);                                             //displaying the power alternatively in Dbm and uW
      lcd.print("mW");
      _delay_ms(700);
      lcd.setCursor(0, 1);
      lcd.print("Power:");
      lcd.print(x, 2);
      lcd.print("dbm  ");
      _delay_ms(700);
    }
      
  }

}
