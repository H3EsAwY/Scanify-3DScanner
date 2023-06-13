int irPin = A1; // define the analog pin for IR sensor
float distance; // variable to hold the distance value

void setup() {
  Serial.begin(9600); // initialize serial communication at 9600 bits per second
}

void loop() {
     int sensorValue = analogRead(irPin); // read the analog value from the IR sensor
  float voltage = sensorValue * 5.0 / 1023.0; // convert analog value to voltage
  distance = 26.2 * pow(voltage, -1.15); // calculate the distance in cm using the formula provided by Sharp datasheet
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");
  delay(1000); // wait for 1 second before taking the next reading


}
