const int dirPin1 = 2;
const int stepPin1 = 3;
const int stepsPerRevolution1 = 200;
const int dirPin2 = 4;
const int stepPin2 = 5;


void setup() {
// Declare pins as Outputs
  pinMode(stepPin1, OUTPUT);
  pinMode(dirPin1, OUTPUT);
  pinMode(stepPin2, OUTPUT);
  pinMode(dirPin2, OUTPUT);
}

void loop() {
   // Set motor direction clockwise
  digitalWrite(dirPin1, HIGH);
  digitalWrite(dirPin2, HIGH);

 
  // Spin motor slowly
  for(int x = 0; x < stepsPerRevolution1; x++)
  { 
    
    digitalWrite(stepPin1, HIGH);
    delayMicroseconds(2000);
    digitalWrite(stepPin1, LOW);
    delayMicroseconds(2000);
 
    
    digitalWrite(stepPin2, HIGH);
    delayMicroseconds(2000);
    digitalWrite(stepPin2, LOW);
    delayMicroseconds(2000);
  }


}
