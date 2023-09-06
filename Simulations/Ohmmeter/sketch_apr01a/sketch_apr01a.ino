#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,16,2); 

float Vout_ohm, new_Vout_ohm, Vref_div_newVout_ohm, R2, Vdc ;

int Vref = 4.85;
float R1K = 120;

void setup() {
  Serial.begin(9600) ;
  lcd.init();
  lcd.backlight();
  pinMode(A0, INPUT) ;
  lcd.setCursor(4,0);
  lcd.print("RESISTOR");
  lcd.setCursor(5,1);
  lcd.print("METER");
  delay(2000);

}

void loop() {
  R2 = 0 ;
  for (int i=0; i < 1000; i++) {
  Vout_ohm = analogRead(A0);
    new_Vout_ohm = (Vout_ohm*Vref) / 1024 ;
    Vref_div_newVout_ohm = Vref / new_Vout_ohm ;
    R2 += (R1K) / (Vref_div_newVout_ohm - 1) ;}
    R2 = R2/1000.0 ;
    if (new_Vout_ohm > 3.4) {R2 += R2*0/100.0;}
    Serial.println(new_Vout_ohm);
    
    if (new_Vout_ohm > 3.8) {
      lcd.clear();
      lcd.setCursor(0,0); 
      lcd.print(" Increase scale");
      lcd.setCursor(0,1); 
      lcd.print("or Insert Resist");
      delay(1000);
             
      }
  
    else {
    if (R2<1){
      lcd.clear();
      R2 = R2*1000;
      lcd.setCursor(0,0);
      lcd.print("Resistance");
      lcd.setCursor(0,1);
      lcd.print(R2);
      lcd.setCursor(5,1);
      lcd.print(" Ohms");
      delay(1000);}
    else if (R2>1) {
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Resistance");
      lcd.setCursor(0,1);
      lcd.print(R2);
      lcd.setCursor(6,1);
      lcd.print("K Ohms");
      delay(1000);
      }
   }
}
