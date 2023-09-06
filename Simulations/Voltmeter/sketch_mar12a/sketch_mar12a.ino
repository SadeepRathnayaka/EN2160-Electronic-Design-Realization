#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,16,2);// Define analog input


#define ANALOG_IN_PIN1 A2
#define ANALOG_IN_PIN2 A3

// Floats for ADC voltage & Input voltage
float adc_voltage1 = 0.0;
float in_voltage1 = 0.0;
float adc_voltage2 = 0.0;
float in_voltage2 = 0.0;
 
 
// Floats for resistor values in divider (in ohms)
float R1_VM = 30000.0;
float R2_VM = 7500.0; 

// Integer for ADC value
int adc_value1 = 0;
int adc_value2 = 0;

// Voltage difference
float voltage_diff = 0 ;

// Voltage reference
float Vref = 4.85 ;



void setup(){
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  // Use external voltage reference
  //analogReference(EXTERNAL);
  
   // Setup Serial Monitor
   
   
   pinMode(10, INPUT);

   lcd.setCursor(4,0);
  lcd.print("VOLTMETER");
  delay(1000);
}
 
void loop(){
  if (digitalRead(10)==HIGH){
    Serial.println("voltmeter");

    //if (analogRead(A1) == HIGH) {
     // Serial.println("A1 is high");
      float dc = analogRead(A1) * Vref * 11 / 1023.0 ;
      lcd.setCursor(0,1);
    lcd.print(voltage_diff);
    lcd.setCursor(6,1);
    lcd.print("volts");
      Serial.print("Voltage = ");
      Serial.println(dc);
      //}
    //if (analogRead(A2) == HIGH && analogRead(A3) == HIGH) {
      Serial.println("A2 and A3 are high") ;
      voltmeter();
     // }

  delay(1000);
}}

void voltmeter(){
  // Read the Analog Input
   adc_value1 = analogRead(ANALOG_IN_PIN1);
   adc_value2 = analogRead(ANALOG_IN_PIN2);
   
   
   // Determine voltage at ADC input
   adc_voltage1  = (adc_value1 * Vref) / 1023.0;
   adc_voltage2  = (adc_value2 * Vref) / 1023.0; 
   
   // Calculate voltage at divider input
   in_voltage1 = adc_voltage1 / (R2_VM/(R1_VM+R2_VM)); 
   in_voltage2 = adc_voltage2 / (R2_VM/(R1_VM+R2_VM));  

   voltage_diff = in_voltage1 - in_voltage2 ;
   
   // Print results to Serial Monitor to 2 decimal places
   
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Voltmeter");
  if (voltage_diff<0.0) {
    lcd.setCursor(0,1);
    lcd.print("Check Polarity !");
   }
  else {
    lcd.setCursor(0,1);
    lcd.print(voltage_diff);
    lcd.setCursor(6,1);
    lcd.print("volts"); 
    Serial.print("Vol difference");
    Serial.println(voltage_diff);
  }
  
  delay(1000);
  }
