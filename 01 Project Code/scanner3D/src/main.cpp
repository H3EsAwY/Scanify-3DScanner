/******************************************************************************
 * Project: 3D Scanner - MTE2024 Project 1
 *
 * ****************** Project Specific Behaviour Description ******************
 *
 ******************************************************************************/

/*----------------------------------------------------------------------------*/
/*------------------------------- Header Files -------------------------------*/
/*----------------------------------------------------------------------------*/
#include <Arduino.h>

/*Arduino Libraries*/
#include <SPI.h>
#include <SD.h>
#include <Wire.h>
// #include <LiquidCrystal_I2C.h>

/*----------------------------------------------------------------------------*/
/*------------------------------- Global -------------------------------------*/
/*----------------------------------------------------------------------------*/

/* Texas Inst. I2C address is 0x27 NXP Semi. I2C address is 0x3F */
// #define i2c_address 0x3F

// LiquidCrystal_I2C lcd(i2c_address,16,2);

String SD_fileName = "scan_001.txt"; // Name of the saved file on the SD card

/*---------------- Calibration and Tweaks ----------------*/

/*-- Scan Resolution --*/
int IR_scans_number = 45;   // Amount of scans for each point. The mean is then calculated. This would increase the delay for each scan, but increase accuracy.
float z_layer_height = 0.5; // in mm           //Layer height. The amount of mm for each layer.
/*-- Stepper Motor Parameters --*/
int step_delay = 47 ;                  // in us                //Delay for each step for the stepper motor in microseconds
int steps_per_rotation_for_motor = 6400; // Steps that the motor needs for a full rotation 200/(1/32).

/*-- Physical Design Specific --*/
int lead_screw_rotations_per_cm = 8; // Lead = NumStarts * Pitch How many rotations needs the lead screw to make in order to make 1cm.
int z_axis_max_height = 8;              // in cm               //Maximum height of the scanned file

int distance_to_center = 18;         // In cm. Distance from sensor to the turntable center in cm

/*----------------------- Variables -----------------------*/
File fileContent;

float distance = 0; // Measured distance from IR_SHARP
float angle = 0;    // Rotation angle for each loop (0º-360º)
float x = 0;        // X Coordinate of the measured point
float y = 0;        // y Coordinate of the measured point
float z = 0;        // z Coordinate of the measured point

int z_loop = 0; // variable used for the z-axis motor rotation
int r_loop = 0; // variable used for the turntable motor rotation
float currentSteps_Z = 0;

float measured_analog = 0; // Analog read from the distance sensor
float analog = 0;          // Analog MEAN

/*---- To be later calculated ----*/
float RADIANS = 0.0;    // Minimum Rotation Angle (stepper resolution) in Rad (1.8 deg). We calculate this value later in Setup loop
int steps_z_height = 0; // Variable used for the amount of steps in z-axis

/*---- Flags ----*/
bool isScanning = 1; // if push button is pressed toggle this flag
bool isFinished = 0; // if scan is finished ( reached max height) set this flag to 1

/*----------------------------------------------------------------------------*/
/*------------------------- Function Prototypes ------------------------------*/
/*----------------------------------------------------------------------------*/

void rotateStepper();
double getCoordXY();
float mapFloat(float fval, float val_in_min, float val_in_max, float val_out_min, float val_out_max);
void write_to_SD(float SDx, float SDy, float SDz);

void setup()
{
  /*----- Pin Configuration ----------------*/

#define IR_SHARP A1
#define SD_ChipSelect 10
#define pushButton 7

// Rotating Stepper (Turntable Stepper) Driver Pins
#define stepperRot_dir 2
#define stepperRot_step 3

// Z-Axis Stepper (Vertical Stepper) Driver Pins
#define stepperZ_dir 4
#define stepperZ_step 5

  /*--- Pin Mode ---*/
  pinMode(IR_SHARP, INPUT);
  pinMode(pushButton, INPUT);

  pinMode(stepperRot_dir, OUTPUT);
  pinMode(stepperRot_step, OUTPUT);

  pinMode(stepperZ_dir, OUTPUT);
  pinMode(stepperZ_step, OUTPUT);

  SD.begin(SD_ChipSelect);

  /*-- Calculating Variables --*/
  RADIANS = (3.141592 / 180.0) * (360 / steps_per_rotation_for_motor); // 0.05625 degress
  steps_z_height = (z_layer_height * steps_per_rotation_for_motor * lead_screw_rotations_per_cm) / 10;
}

void loop()
{

  /*Enable for push button*/
  /*if button is pushed, start scanning*/
  /*
  if (digitalRead(pushButton))
  {
    isScanning = !isScanning; // toggle flag
    delay(3000);              // give time for pushButton to be unpressed
  }
  */
  if (isScanning == 1)
  {
    // Unless we reach the maximum height of the project, keep running the 2 stepper loops
    if (z < z_axis_max_height)
    {

      /* Rotating Stepper Loop: Performs 1  */
      for (int loop_cont = 0; loop_cont < steps_per_rotation_for_motor; loop_cont++)
      {
        getCoordXY();

        digitalWrite(stepperRot_dir, HIGH);   // turntable spin to counter clockwise
        digitalWrite(stepperRot_step, HIGH); // make a step
        delayMicroseconds(step_delay);
        digitalWrite(stepperRot_step, LOW);
        delayMicroseconds(step_delay);

        angle = angle + RADIANS; // Increase the angle by one more unit

        write_to_SD(x, y, z); // Write x, y, z files to SD card function

        // Uncomment this for Serial debug
        /*
        Serial.print(loop_cont);     Serial.print("   ");
        Serial.print(angle);     Serial.print("   ");
        Serial.print(distance);     Serial.print("   ");
        Serial.print(x);     Serial.print("   ");
        Serial.print(y);     Serial.print("   ");
        Serial.print(z);     Serial.println("   "); */
      }

      angle = 0; // Reset the angle value for next rotation

      /*My threaded rod needs 8 full rotations for 1cm. A full rotation is 200 steps in my case.
      We need 1600 for 1cm. So, we need 80 for 0.5mm. The amount is calulated automaically.
      Just change the variables at the beginning if you want*/

      while (z_loop < steps_z_height)
      {

        digitalWrite(stepperZ_dir, HIGH);   // z_azis spin Clockwise
        digitalWrite(stepperZ_step, HIGH); // z_azis make a step
        delayMicroseconds(step_delay);
        digitalWrite(stepperZ_step, LOW);
        delayMicroseconds(step_delay);
        z_loop = z_loop + 1; // Increase the loop by 1
      }

      z = z + z_layer_height; // Increase the made z-height by 1 unit
      currentSteps_Z = currentSteps_Z + steps_z_height;
      z_loop = 0;             // Reset the z-axis rotation variable

    } // end of if z_height

    // We finished the scan, we go to home position
    else
    {
      /*go to home position*/
      while(currentSteps_Z>0)
      {
      digitalWrite(stepperZ_dir, LOW);   // z_azis spin counter Clockwise
      digitalWrite(stepperZ_step, HIGH); // z_azis make a step
      delayMicroseconds(step_delay);
      digitalWrite(stepperZ_step, LOW);
      delayMicroseconds(step_delay);
      currentSteps_Z-- ; // decrease the loop by 1
      }
    }
  }
}

// Function that gets the distance from sensor
double getCoordXY()
{
  for (int scanCount = 0; scanCount < IR_scans_number; scanCount++)
  {
    measured_analog = analogRead(A0);
    //delay(2);
    analog = analog + measured_analog;
  }
  distance = analog / IR_scans_number; // Get the mean. Divide the scan by the amount of scans.
  analog = 0;                          // reset the analog read total value
  measured_analog = 0;                 // reset the analog read value

  distance = mapFloat(distance, 0.0, 1023.0, 0.0, 3.3);                                               // Convert analog pin reading to voltage
  distance = -5.40274 * pow(distance, 3) + 28.4823 * pow(distance, 2) - 49.7115 * distance + 31.3444; // From datasheet

  distance = distance_to_center - distance; // the distance d = distance from sensor to center - measured distance (thus distance is the distance from center.)

  y = (cos(angle) * distance);
  x = (sin(angle) * distance);

  /*//For debug
   * Serial.print(distance); Serial.print("   ");
  Serial.print(x); Serial.print("   ");
  Serial.print(y); Serial.print("   ");
  Serial.print(z); Serial.print("   ");
  Serial.print(angle); Serial.println("   "); */

  return 0;
}

// Function that maps the value in a float format
float mapFloat(float fval, float val_in_min, float val_in_max, float val_out_min, float val_out_max)
{
  return (fval - val_in_min) * (val_out_max - val_out_min) / (val_in_max - val_in_min) + val_out_min;
}

// Function that writes the value to the SD card
void write_to_SD(float SDx, float SDy, float SDz)
{
  fileContent = SD.open(SD_fileName, FILE_WRITE);
  if (fileContent)
  {
    fileContent.print(x);
    fileContent.print(",");
    fileContent.print(y);
    fileContent.print(",");
    fileContent.println(z);
    fileContent.close();
  }
}
