#include <Wire.h>
#include <Adafruit_INA219.h>

Adafruit_INA219 ina219;


void setup(void) 
{
  Serial.begin(115200);
  while (!Serial) {
      // will pause Zero, Leonardo, etc until serial console opens
      delay(1);
  }
    
  Serial.println("Hello!");
  
  // Initialize the INA219.
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
}

void loop(void) 
{
  ammeter()
}

void ammeter(){
  float current_mA = 0;
  current_mA = ina219.getCurrent_mA();
  
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Ammeter");
    lcd.setCursor(0,1);
    lcd.print(current_mA);
    lcd.setCursor(6,1);
    lcd.print("mA"); 
  }
