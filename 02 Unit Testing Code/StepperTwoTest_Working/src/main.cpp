/*
 * Notes Written By: Hany Elesawy
 * 
 * Microstepping of 1/32 was applied on hardware level
 * 
 * So A complete revolution isn't just 200 steps any more
 * 
 * Complete 360 degrees revolution = 200/(1/32) = 6400 steps for a complete revolution
 * 
 * 
 * 
 */


#include <Arduino.h>

#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2); // I2C address 0x27, 16 column and 2 rows

const int dirPin1 = 2;
const int stepPin1 = 3;
const int stepsPerRevolution1 = 200;
const int dirPin2 = 4; 
const int stepPin2 = 5;

 
void setup()
{
  // Declare pins as Outputs

  /*Turn Table Stepper*/
  pinMode(stepPin1, OUTPUT);
  pinMode(dirPin1, OUTPUT);

  /*Z-Axis Stepper*/
  pinMode(stepPin2, OUTPUT);
  pinMode(dirPin2, OUTPUT);
  lcd.init(); // initialize the lcd
  lcd.backlight();

  //Serial.begin(9600);

}
void loop()
{
  

  // Set motor direction clockwise
  /*Turn Table Stepper*/
  digitalWrite(dirPin1, HIGH);
  /*Z-Axis Stepper*/
  digitalWrite(dirPin2, LOW);

 
  // Spin motor slowly
  /*
  for(int x = 0; x < 6400; x++)
  { 
    Serial.print("RotStep Number: ");
    Serial.println(x);

    digitalWrite(stepPin1, HIGH);
    delayMicroseconds(47);
    digitalWrite(stepPin1, LOW);
    delayMicroseconds(47);

  }
  */
  
    for(int y = 0; y <6400; y++)
  { 
    Serial.print("Z-AxisStep Number: ");
    Serial.println(y);
    digitalWrite(stepPin2, HIGH);
    delayMicroseconds(5);
    digitalWrite(stepPin2, LOW);
    delayMicroseconds(5);
 
  }

  

  
}