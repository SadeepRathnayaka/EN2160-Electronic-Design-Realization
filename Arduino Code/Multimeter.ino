#include <Wire.h>
#include <Adafruit_INA219.h>
#include <LiquidCrystal_I2C.h>

#define INA219_ADDRESS 0x40
#define LCD_ADDRESS 0x27

int delay_time = 100;

LiquidCrystal_I2C lcd(LCD_ADDRESS,16,2); 
Adafruit_INA219 ina219;


////////////////////////////////////////// Mode Rotary Switch Inputs /////////////////////////////////////

int VOLTMETER = 10;
int AMMETER = 11;
int OHMMETER = 12;
int CONTINUITY = 13;

////////////////////////////////////////// ohmmeter ///////////////////////////////////////////////////////
// rotary switch inputs
int R1K_input = 8 ;  // 0 - 5K
int R10K_input = 9 ;  // 5K - 40K
int R27K_input = 2 ;  // 40K - 100K
int R100K_input = 3 ; // 100K - 450K

// selecting scale pins
int R1K_output = 4 ;
int R10K_output = 5 ;
int R27K_output = 6 ;
int R100K_output = 7 ;


// resistors values
float R1K = 1.0;
float R10K  = 10.0 ;
float R27K  = 30.0 ;
float R100K = 120.0 ;

///////////////////////////////////// end of ohmmeter ///////////////////////////////////////////////////


////////////////////////////////////////Voltmeter //////////////////////////////////////////////////////
int res1_vol = 1000 ;
int res2_vol = 100 ;
int diode_val = 0.6 ;
//////////////////////////////////////End of Voltmeter /////////////////////////////////////////////////


// reference voltage given from buck converter
int Vref = 5;

// constants oh ohmmeter
float Vout_ohm, new_Vout_ohm, Vref_div_newVout_ohm, R2 ;

void setup() {
  
  // analog reference and lcd displlay initializing
  //analogReference(EXTERNAL);
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(4,0);
  lcd.print("MULTIMETER");
  delay(3000);
  

  ////////////////////////////// INA219 initializing ///////////////////////////
  Serial.begin(115200);
  while (!Serial) {
      // will pause Zero, Leonardo, etc until serial console opens
      delay(1);
  }
    
  //Serial.println("Hello!");
  

  if (! ina219.begin()) {
    Serial.println("Failed to find INA219 chip");
    while (1) { delay(10); }
  }
  // To use a slightly lower 32V, 1A range (higher precision on amps):
 // ina219.setCalibration_32V_1A();
  // Or to use a lower 16V, 400mA range (higher precision on volts and amps):
  //ina219.setCalibration_16V_400mA();

 // Serial.println("Measuring voltage and current with INA219 ...");
/////////////////////////////////End of INA219 /////////////////////////////////////

  // Mode Rotary Switch pins initializing
  pinMode(VOLTMETER,INPUT);
  pinMode(AMMETER,INPUT);
  pinMode(OHMMETER,INPUT);
  pinMode(CONTINUITY,INPUT);

   
  // ohmmeter pins intializing
  pinMode(R1K_input,INPUT);
  pinMode(R10K_input,INPUT);
  pinMode(R27K_input,INPUT);
  pinMode(R100K_input,INPUT);

  pinMode(R1K_output,INPUT);
  pinMode(R10K_output,INPUT);
  pinMode(R27K_output,INPUT);
  pinMode(R100K_output,INPUT);

  pinMode(A0, INPUT);  // Vout_ohm reading pin

  // end of ohmmeter initializing

}

void loop() {

    if (digitalRead(VOLTMETER) == HIGH) {
    Serial.println("voltmeter");
      pinMode(VOLTMETER,INPUT);
      pinMode(AMMETER,OUTPUT);
      pinMode(OHMMETER,OUTPUT);
      pinMode(CONTINUITY,OUTPUT);

      lcd.clear();
      lcd.setCursor(4,0);
      lcd.print("VOLTMETER");
      delay(1000);

    while (true) {
    voltmeter();
    if (digitalRead(VOLTMETER) != HIGH) {
      delay(delay_time);
      break;}
    }
  
}


  if (digitalRead(AMMETER) == HIGH) {
    Serial.println("ammeter");
      pinMode(VOLTMETER,OUTPUT);
      pinMode(AMMETER,INPUT);
      pinMode(OHMMETER,OUTPUT);
      pinMode(CONTINUITY,OUTPUT);

      lcd.clear();
      lcd.setCursor(4,0);
      lcd.print("AMMETER");
     // delay(1000);

    while (true) {
    ammeter();
    if (digitalRead(AMMETER) != HIGH) {
      delay(delay_time);
      break;}
    }
  
}


  if (digitalRead(OHMMETER) == HIGH) {
    Serial.println("OHMmeter");
      pinMode(VOLTMETER,OUTPUT);
      pinMode(AMMETER,OUTPUT);
      pinMode(OHMMETER,INPUT);
      pinMode(CONTINUITY,OUTPUT);

      lcd.clear();
      lcd.setCursor(4,0);
      lcd.print("OHMMETER");
      delay(1000);

  while (true) {
    ohmmeter();
    if (digitalRead(OHMMETER) != HIGH) {
      delay(delay_time);
      break;}
    }
    }

  if (digitalRead(CONTINUITY) == HIGH) {
    Serial.println("continuity");
      pinMode(VOLTMETER,OUTPUT);
      pinMode(AMMETER,OUTPUT);
      pinMode(OHMMETER,OUTPUT);
      pinMode(CONTINUITY,INPUT);
    
      lcd.clear();
      lcd.setCursor(4,0);
      lcd.print("CONTINUITY");
      lcd.setCursor(4,1);
      lcd.print("CHECKER");
      delay(1000);

    while (true) {
    if (digitalRead(CONTINUITY) != HIGH) {
      delay(delay_time);
      break;}
    }
    }






}


void ohmmeter() {
  
    // 0 - 5K
  if (digitalRead(R1K_input) == HIGH) {
    
    pinMode(R1K_output,OUTPUT);
    pinMode(R10K_output,INPUT);
    pinMode(R27K_output,INPUT);
    pinMode(R100K_output,INPUT);
    digitalWrite(R1K_output, HIGH);


    R2 = 0 ;
  for (int i=0; i < 1000; i++) {
    Vout_ohm = analogRead(A0);
    new_Vout_ohm = (Vout_ohm*Vref) / 1024 ;
    Vref_div_newVout_ohm = Vref / new_Vout_ohm ;
    R2 += (R1K) / (Vref_div_newVout_ohm - 1) ;}
    R2 = R2/1000.0 ;
    Serial.print("1K  ");
    Serial.println(new_Vout_ohm);
    
    if (new_Vout_ohm > 4.2) {
      lcd.clear();
      lcd.setCursor(0,0); 
      lcd.print("Increase scale /");
      lcd.setCursor(0,1); 
      lcd.print("Change Resistor");
      while (true) {
        if (digitalRead(R1K_input) != HIGH){
          delay(delay_time);
          break;}
        Vout_ohm = analogRead(A0);
        new_Vout_ohm = (Vout_ohm*Vref) / 1024 ;
        if (new_Vout_ohm < 4.00) {
          delay(delay_time);
          break;}
          }       
      }
  
   else {
    if (R2<1){
      lcd.clear();
      
      R2 = R2*1000;
      
//      if (R2 < 10 ) {R2 += R2 * 0.4;}
//      else if (10 < R2 && R2 < 250 ) {R2 += R2 * 0.08;}
//      else if (250 < R2 && R2 < 600 ) {R2 += R2 * 0.07;}
//      else if (600 < R2  && R2 < 800 ) {R2 += R2 * 0.06;}
//      else  {R2 += R2 * 0.04;}
      
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
      lcd.setCursor(5,1);
      lcd.print("K Ohms");
      delay(1000);
      }
   }
}

//   5K - 95K
  if (digitalRead(R10K_input) == HIGH) {
    
    pinMode(R1K_output,INPUT);
    pinMode(R10K_output,OUTPUT);
    pinMode(R27K_output,INPUT);
    pinMode(R100K_output,INPUT);
    digitalWrite(R1K_output, 0);
    digitalWrite(R10K_output, 1);
    digitalWrite(R27K_output, 0);
    digitalWrite(R100K_output, 0);
    
    R2 = 0 ;
  for (int i=0; i < 1000; i++) {
    Vout_ohm = analogRead(A0);
    new_Vout_ohm = (Vout_ohm*Vref) / 1024 ;
    Vref_div_newVout_ohm = Vref / new_Vout_ohm ;
    R2 += (R10K) / (Vref_div_newVout_ohm - 1) ;}
    R2 = R2/1000.0 ;
    Serial.print("10K  ");
    Serial.println(new_Vout_ohm);
    
    if (new_Vout_ohm > 4.56) {
      lcd.clear();
      lcd.setCursor(0,0); 
      lcd.print("Increase scale /");
      lcd.setCursor(0,1); 
      lcd.print("Change Resistor");       
      delay(1000);
      while (true) {
        if (digitalRead(R10K_input) != HIGH){
          delay(delay_time);
          break;}
        Vout_ohm = analogRead(A0);
        new_Vout_ohm = (Vout_ohm*Vref) / 1024 ;
        if (new_Vout_ohm < 4.56) {
          delay(delay_time);
          break;}
          } 
      }
    else if (new_Vout_ohm < 1.66 && new_Vout_ohm > 0.5 ) {
      lcd.clear();
      lcd.setCursor(0,0); 
      lcd.print(" Decrease scale");     
      delay(1000);
      while (true) {
        if (digitalRead(R10K_input) != HIGH){
          delay(delay_time);
          break;}

        Vout_ohm = analogRead(A0);
        new_Vout_ohm = (Vout_ohm*Vref) / 1024 ;
        if (new_Vout_ohm > 1.68 ) {
          delay(delay_time);
          break;}
      }
      }
    else {
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Resistance");
      lcd.setCursor(0,1);
      lcd.print(R2);
      lcd.setCursor(5,1);
      lcd.print("K Ohms");
      delay(1000);
    }}
//
  // 99K - 250K
  if (digitalRead(R27K_input) == HIGH) {
    
    pinMode(R1K_output,INPUT);
    pinMode(R10K_output,INPUT);
    pinMode(R27K_output,OUTPUT);
    pinMode(R10K_output,INPUT);
    digitalWrite(R27K_output, HIGH);

    R2 = 0 ;
  for (int i=0; i < 1000; i++) {
    Vout_ohm = analogRead(A0);
    new_Vout_ohm = (Vout_ohm*Vref) / 1024 ;
    Vref_div_newVout_ohm = Vref / new_Vout_ohm ;
    R2 += (R27K) / (Vref_div_newVout_ohm - 1) ;}
    R2 = R2/1000.0 ;
    Serial.print("27K  ");
    Serial.println(new_Vout_ohm);
    
     if (new_Vout_ohm > 4.55) {
      lcd.clear();
      lcd.setCursor(0,0); 
      lcd.print("Increase scale /");
      lcd.setCursor(0,1); 
      lcd.print("Insert Resistor");       
      delay(1000);
      while (true) {
        if (digitalRead(R27K_input) != HIGH){
          delay(delay_time);
          break;}
        Vout_ohm = analogRead(A0);
        new_Vout_ohm = (Vout_ohm*Vref) / 1024 ;
        if (new_Vout_ohm < 4.550) {
          delay(delay_time);
          break;}
          } 
      }

    else if (new_Vout_ohm < 3.85 && new_Vout_ohm > 0.8) {
      lcd.clear();
      lcd.setCursor(0,0); 
      lcd.print(" Decrease scale");     
      delay(1000);
      while (true) {
        if (digitalRead(R27K_input) != HIGH){
          delay(delay_time);
          break;}
        Vout_ohm = analogRead(A0);
        new_Vout_ohm = (Vout_ohm*Vref) / 1024 ;
        if (new_Vout_ohm > 3.88 ) {
          delay(delay_time);
          break;}
      }}
    else {
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Resistance");
      lcd.setCursor(0,1);
      lcd.print(R2);
      lcd.setCursor(5,1);
      lcd.print("K Ohms");
      delay(1000);
    }}

//  300K - 1M
  if (digitalRead(R100K_input) == HIGH) {
    
    pinMode(R1K_output,INPUT);
    pinMode(R10K_output,INPUT);
    pinMode(R27K_output,INPUT);
    pinMode(R100K_output,OUTPUT);
    digitalWrite(R100K_output, HIGH);
    
    R2 = 0 ;
  for (int i=0; i < 1000; i++) {
    Vout_ohm = analogRead(A0);
    new_Vout_ohm = (Vout_ohm*Vref) / 1024 ;
    Vref_div_newVout_ohm = Vref / new_Vout_ohm ;
    R2 += (R100K) / (Vref_div_newVout_ohm - 1) ;}
    R2 = R2/1000.0 ;
    Serial.print("120K  ");
    Serial.println(new_Vout_ohm);
    
     if (new_Vout_ohm > 4.5) {
      lcd.clear();
      lcd.setCursor(0,0); 
      lcd.print("Out of Range /");
      lcd.setCursor(0,1); 
      lcd.print("Insert Resistor");       
      delay(1000);
      while (true) {
        if (digitalRead(R100K_input) != HIGH){
          delay(delay_time);
          break;}
        Vout_ohm = analogRead(A0);
        new_Vout_ohm = (Vout_ohm*Vref) / 1024 ;
        if (new_Vout_ohm < 4.50) {
          delay(delay_time);
          break;}
          } 
      }

      else if (new_Vout_ohm < 3.55 && new_Vout_ohm > 0.5) {
      lcd.clear();
      lcd.setCursor(0,0); 
      lcd.print(" Decrease scale");     
      delay(1000);
      while (true) {
        if (digitalRead(R100K_input) != HIGH){
          delay(delay_time);
          break;}
        Vout_ohm = analogRead(A0);
        new_Vout_ohm = (Vout_ohm*Vref) / 1024 ;
        if (new_Vout_ohm > 3.56 ) {
          delay(delay_time);
          break;}
      }}
    else {
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Resistance");
      lcd.setCursor(0,1);
      lcd.print(R2);
      lcd.setCursor(5,1);
      lcd.print("K Ohms");
      delay(1000);
    }}
  }

void voltmeter() {
  float dc = 0 ;
  for(int i=0; i<1000; i++) {
    dc += (analogRead(A1)*4.85*11/1023);
  }
  dc = dc / 1000 ;

  if (dc == 0 ) {dc += 0 ;}
  else if (dc > 0 && dc < 1) {dc += 0 ;}
  else if (dc > 1 && dc < 4) {dc += 0.25 ;}
  else if (dc >4 && dc < 5) {dc += 0.28;}
  else if (dc > 5 && dc < 6) {dc += 0.3;}
  else if (dc > 6 && dc < 8) {dc += 0.33;}
  else if (dc > 8 && dc < 10) {dc += 0.34;}
  else if (dc > 10 && dc < 12) {dc += 0.35;}
  else if (dc > 12 && dc < 16) {dc += 0.36;}
  else if (dc > 16 && dc < 22) {dc += 0.37;}
  else if (dc > 22 && dc < 25) {dc += 0.38;}
  else {dc += 0.39;}

  Serial.println(dc);
  // float dc = (analogRead(A1)*4.85*11/1023) ;
  if (dc > 0 && dc < 1){
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Voltage is not");
    lcd.setCursor(0,1);
    lcd.print("in Range !");
    delay(1000);}
  else {
    dc += 0.075 ;
    lcd.clear();
    Serial.println(dc);
    lcd.setCursor(0,1);
    lcd.print(dc);
    lcd.setCursor(6,1);
    lcd.print("volts");}
  delay(1000);}

void ammeter(){
  float current_mA = 0;

  for(int i=0; i<5000; i++) {
  current_mA += ina219.getCurrent_mA();}
  current_mA = current_mA / 5000.0 ;

  if (current_mA <= 0 ) {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Ammeter");
    lcd.setCursor(0,1);
    lcd.print("Check Probes !"); 
    //delay(1000);
    }
  else { 
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Ammeter");
    lcd.setCursor(0,1);
    lcd.print(current_mA);
    lcd.setCursor(6,1);
    lcd.print("mA"); 
  //delay(1000);
  }
  }
