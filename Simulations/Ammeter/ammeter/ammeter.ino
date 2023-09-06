#include <Wire.h>
#include <Adafruit_INA219.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,16,2);// Define analog input


Adafruit_INA219 ina219;


void setup(void) 
{
  lcd.init();
  lcd.backlight();
  Serial.begin(11200);
  while (!Serial) {
      // will pause Zero, Leonardo, etc until serial console opens
      delay(1);
  }
   
  
  
  // By default the initialization will use the largest range (32V, 2A).  However
  // you can call a setCalibration function to change this range (see comments).
  if (! ina219.begin()) {
    Serial.println("Failed to find INA219 chip");
    while (1) { delay(10); }
  }
  // To use a slightly lower 32V, 1A range (higher precision on amps):
  ina219.setCalibration_32V_1A();
  // Or to use a lower 16V, 400mA range (higher precision on volts and amps):
  //ina219.setCalibration_16V_400mA();

  Serial.println("Measuring voltage and current with INA219 ...");
  lcd.setCursor(4,0);
  lcd.print("AMMETER");
}

void loop(void) 
{
  float shuntvoltage = 0;
  float busvoltage = 0;
  float current_mA = 0;
  float loadvoltage = 0;
  float power_mW = 0;

  
 for (int i=0; i<1000; i++){
  current_mA += ina219.getCurrent_mA();}
  current_mA = current_mA / 1000.0;
 
 
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Ammeter");
  lcd.setCursor(0,1);
  lcd.print(current_mA);
  lcd.setCursor(6,1);
  lcd.print("mA");
  delay(1000);
  

}
