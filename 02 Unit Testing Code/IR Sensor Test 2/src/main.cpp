#include <Arduino.h>

int irPin = A1; // define analog pin 1 "A1" for IR sensor
float distance; // declare variable to hold distancd value

float mapFloat(float fval, float val_in_min, float val_in_max, float val_out_min, float val_out_max);
void setup()
{
  Serial.begin(9600); // initialize serial communication at 9600 bits/second
}

void loop()
{
  int sensorValue = analogRead(irPin);                  // read analog value from ir sensor
  distance = mapFloat(distance, 0.0, 1023.0, 0.0, 3.3); // Convert analog pin reading to voltage
  /* Scale The distance to 0 - 3.3 Volts */
  distance = -5.40274 * pow(distance, 3) + 28.4823 * pow(distance, 2) - 49.7115 * distance + 31.3444; // calculate distance in cm using the formula
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");
  delay(1000);
}

float mapFloat(float fval, float val_in_min, float val_in_max, float val_out_min, float val_out_max)
{
  return (fval - val_in_min) * (val_out_max - val_out_min) / (val_in_max - val_in_min) + val_out_min;
}
