#include <Arduino.h>
#include <SD.h>
#include <SPI.h>
#include <Wire.h>

File myFile;

int pinCS = 10; // pin 10 on arduino nano

void setup()
{
  Serial.begin(9600);
  pinMode(pinCS, OUTPUT);

  // SD card initialization
  if (SD.begin())
  {
    Serial.println("SD ready");
  }
  else
  {
    Serial.println("SD failed");
    return;
  }

  // create/open file
  myFile = SD.open("test.text", FILE_WRITE);

  // if file is opened okay
  if (myFile)
  {
    Serial.println("Writing to file.... ");
    // write to file
    myFile.println("Testing text 1, 2, 3...");
    myFile.close(); // close the file
    Serial.println("Done");
  }
  // if file didn't open
  else
  {
    Serial.println("error opening test.txt");
  }

  // reading the file
  myFile = SD.open("test.txt");

  if (myFile)
  {
    Serial.println("Read:");
    // reading the whole file
    while (myFile.available())
    {
      Serial.write(myFile.read());
    }
    myFile.close();
  }
  else
  {
    Serial.println("error opening test.txt");
  }
}

void loop()
{
  // empty
}
