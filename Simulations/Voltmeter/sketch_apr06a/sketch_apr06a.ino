#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,16,2);// Define analog input

void setup() {
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();

}

void loop() {
  if (analogRead(A1 == HIGH)) {
  float dc = analogRead(A1)*4.85*22/1023;
  if (dc == 0){
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Check Probes or");
    lcd.setCursor(0,1);
    lcd.print("Polarity");
    delay(1000);}
  else {
    lcd.clear();
  if (dc < 1.55){
    lcd.setCursor(0,1);
    lcd.print(dc+0.3);
    lcd.setCursor(6,1);
    lcd.print("volts");}
  else {
  Serial.println(dc);
  lcd.setCursor(0,1);
    lcd.print(dc);
    lcd.setCursor(6,1);
    lcd.print("volts");}
  delay(1000);}
}}
