#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,16,2); 

int R1K_input = 6;  // 0 - 1K    vdc = 1.75
int R10K_input = 5 ;  // 1K - 95K   vdc = 3.60
int R27K_input = 4 ;  // 95K - 180K   vdc = 3.54
int R100K_input = 3 ; // 205K - 2M    vdc = 3.8

// selecting scale pins
int R1K_output = 13 ;
int R10K_output = 12 ;
int R27K_output = 11;
int R100K_output = 10 ;


// resistors values

float R1K  = 1.2 ;
float R10K  = 10.0 ;
float R27K  = 27.0 ;
float R100K = 120.0 ;


int Vref = 4.85;
float Vout_ohm, new_Vout_ohm, Vref_div_newVout_ohm, R2, Vdc ;

void setup() {
  //analogReference(EXTERNAL);
  lcd.init();
  lcd.backlight();
  Serial.begin(9600) ;
  pinMode(R1K_input,INPUT);
  pinMode(R10K_input,INPUT);
  pinMode(R27K_input,INPUT);
  pinMode(R100K_input,INPUT);

  pinMode(R1K_output,INPUT);
  pinMode(R10K_output,INPUT);
  pinMode(R27K_output,INPUT);
  pinMode(R100K_output,INPUT);

  pinMode(A0, INPUT);  // Vout reading pin
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  pinMode(A3, INPUT);
  pinMode(A4, INPUT);
  
  lcd.setCursor(4,0);
  lcd.print("RESISTOR");
  lcd.setCursor(5,1);
  lcd.print("METER");
  delay(2000);

}

void loop() {
    ohmmeter();


    
}


void ohmmeter() {
  
    // 0 - 1K
  if (digitalRead(R1K_input) == HIGH) {
    
    pinMode(R1K_output,OUTPUT);
    pinMode(R10K_output,INPUT);
    pinMode(R27K_output,INPUT);
    pinMode(R100K_output,INPUT);
    digitalWrite(R1K_output, HIGH);
    digitalWrite(R10K_output, 0);
    digitalWrite(R27K_output, 0);
    digitalWrite(R100K_output, 0);


  R2 = 0 ;
  for (int i=0; i < 1000; i++) {
    Vout_ohm = analogRead(A0);
    new_Vout_ohm = (Vout_ohm*Vref) / 1024 ;
    Vref_div_newVout_ohm = Vref / new_Vout_ohm ;
    R2 += (R1K) / (Vref_div_newVout_ohm - 1) ;}
    R2 = R2/1000.0 ;
    
//    Serial.print("1.2K  ");
    Serial.println(new_Vout_ohm);
   
    
    if (new_Vout_ohm > 1.75) {
      lcd.clear();
      lcd.setCursor(0,0); 
      lcd.print(" Increase scale");
      lcd.setCursor(0,1); 
      lcd.print("or Insert Resist");
      while (true) {
        if (digitalRead(R1K_input) != HIGH){
          delay(1000);
          break;}
        Vout_ohm = analogRead(A0);
        new_Vout_ohm = (Vout_ohm*Vref) / 1024 ;
        if (new_Vout_ohm < 4.00) {
          delay(1000);
          break;}
          }       
      }
  
    else {
    if (R2<1){
      lcd.clear();
      Serial.print (R2); Serial.print ("  ");
      R2 = R2*1000;
      Serial.println (R2);
      if (R2 < 10 ) {R2 += R2 * 0.4;}
      else if (10 < R2 && R2 < 250 ) {R2 += R2 * 0.08;}
      else if (250 < R2 && R2 < 600 ) {R2 += R2 * 0.07;}
      else if (600 < R2  && R2 < 800 ) {R2 += R2 * 0.06;}
      else  {R2 += R2 * 0.04;}
      
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


//   1K - 95K
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
    
    if (new_Vout_ohm > 3.60) {
      lcd.clear();
      lcd.setCursor(0,0); 
      lcd.print(" Increase scale");
      lcd.setCursor(0,1); 
      lcd.print("or Insert Resist");       
      delay(1000);
      while (true) {
        if (digitalRead(R10K_input) != HIGH){
          delay(1000);
          break;}
        Vout_ohm = analogRead(A0);
        new_Vout_ohm = (Vout_ohm*Vref) / 1024 ;
        if (new_Vout_ohm < 4.00) {
          delay(1000);
          break;}
          } 
      }
    else if (new_Vout_ohm < 0.5) {
      lcd.clear();
      lcd.setCursor(0,0); 
      lcd.print(" Decrease scale");     
      delay(1000);
      while (true) {
        if (digitalRead(R10K_input) != HIGH){
          delay(1000);
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
  // 99K - 200K
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
    
     if (new_Vout_ohm > 3.54) {
      lcd.clear();
      lcd.setCursor(0,0); 
      lcd.print(" Increase scale");
      lcd.setCursor(0,1); 
      lcd.print("or Insert Resist");       
      delay(1000);
      while (true) {
        if (digitalRead(R27K_input) != HIGH){
          delay(1000);
          break;}
        Vout_ohm = analogRead(A0);
        new_Vout_ohm = (Vout_ohm*Vref) / 1024 ;
        if (new_Vout_ohm < 4.00) {
          delay(1000);
          break;}
          } 
      }

    else if (new_Vout_ohm < 3.13) {
      lcd.clear();
      lcd.setCursor(0,0); 
      lcd.print(" Decrease scale");     
      delay(1000);
      while (true) {
        if (digitalRead(R27K_input) != HIGH){
          delay(1000);
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


//  200K - 2M
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
    
     if (new_Vout_ohm > 3.8) {
      lcd.clear();
      lcd.setCursor(0,0); 
      lcd.print(" Out of Range");
      lcd.setCursor(0,1); 
      lcd.print("or Insert Resist");       
      delay(1000);
      while (true) {
        if (digitalRead(R100K_input) != HIGH){
          delay(1000);
          break;}
        Vout_ohm = analogRead(A0);
        new_Vout_ohm = (Vout_ohm*Vref) / 1024 ;
        if (new_Vout_ohm < 4.00) {
          delay(1000);
          break;}
          } 
      }

      else if (new_Vout_ohm < 1.81) {
      lcd.clear();
      lcd.setCursor(0,0); 
      lcd.print(" Decrease scale");     
      delay(1000);
      while (true) {
        if (digitalRead(R100K_input) != HIGH){
          delay(1000);
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


//void autoOhm() {
//    pinMode(R1K_output,OUTPUT);
//    pinMode(R10K_output,INPUT);
//    pinMode(R27K_output,INPUT);
//    pinMode(R100K_output,INPUT);
//    digitalWrite(R1K_output, HIGH);
//    
//    Vdc = analogRead(A0)*Vref/1024.0 ;
//
//  if (Vdc > 4.5) {
//    pinMode(R1K_output,INPUT);
//    pinMode(R10K_output,OUTPUT);
//    pinMode(R27K_output,INPUT);
//    pinMode(R100K_output,INPUT);
//    digitalWrite(R10K_output, HIGH);
//
//    Vdc = analogRead(A0)*Vref/1024.0 ;
//
//    if (Vdc > 4.5) {
//      pinMode(R1K_output,INPUT);
//      pinMode(R10K_output,INPUT);
//      pinMode(R27K_output,OUTPUT);
//      pinMode(R100K_output,INPUT);
//      digitalWrite(R27K_output, HIGH);
//
//      Vdc = analogRead(A0)*Vref/1024.0 ;
//
//        if (Vdc > 4.5) {
//          pinMode(R1K_output,INPUT);
//          pinMode(R10K_output,INPUT);
//          pinMode(R27K_output,INPUT);
//          pinMode(R100K_output,OUTPUT);
//          digitalWrite(R100K_output, HIGH);
//    
//          Vdc = analogRead(A0)*Vref/1024.0 ;
//          Vref_div_newVout_ohm = Vref / Vdc ;
//          R2 = (R100K) / (Vref_div_newVout_ohm - 1) ;
//          if (new_Vout_ohm > 4.5) {
//          lcd.clear();
//          lcd.setCursor(0,0); 
//          lcd.print(" Out of Range");
//          lcd.setCursor(0,1); 
//          lcd.print("or Insert Resist");       
//          delay(1000);
//          }
//        else {
//          lcd.clear();
//          lcd.setCursor(0,0);
//          lcd.print("Resistance");
//          lcd.setCursor(0,1);
//          lcd.print(R2);
//          lcd.setCursor(5,1);
//          lcd.print("K Ohms");
//          delay(1000);
//    }
//
//          }
//          else {
//          Vref_div_newVout_ohm = Vref / Vdc ;
//          R2 = (R27K) / (Vref_div_newVout_ohm - 1) ;
//          lcd.clear();
//          lcd.setCursor(0,0);
//          lcd.print("Resistance");
//          lcd.setCursor(0,1);
//          lcd.print(R2);
//          lcd.setCursor(5,1);
//          lcd.print("K Ohms");
//          delay(1000);
//          }  
//      }
//    
//    else {
//      Vref_div_newVout_ohm = Vref / Vdc ;
//      R2 = (R10K) / (Vref_div_newVout_ohm - 1) ;
//      lcd.clear();
//      lcd.setCursor(0,0);
//      lcd.print("Resistance");
//      lcd.setCursor(0,1);
//      lcd.print(R2);
//      lcd.setCursor(5,1);
//      lcd.print("K Ohms");
//      delay(1000);
//      }
//    }
//  else {
//    Vref_div_newVout_ohm = Vref / Vdc ;
//    R2 = (R1K) / (Vref_div_newVout_ohm - 1) ;
//      if (R2<1){
//      lcd.clear();
//      R2 = R2*1000;
//      lcd.setCursor(0,0);
//      lcd.print("Resistance");
//      lcd.setCursor(0,1);
//      lcd.print(R2);
//      lcd.setCursor(5,1);
//      lcd.print(" Ohms");
//      delay(1000);}
//    if (R2>1) {
//      lcd.clear();
//      lcd.setCursor(0,0);
//      lcd.print("Resistance");
//      lcd.setCursor(0,1);
//      lcd.print(R2);
//      lcd.setCursor(5,1);
//      lcd.print("K Ohms");
//      delay(1000);
//      }
//    }
//  }
