#include <SoftwareSerial.h>

//Create software serial object to communicate with HC-05
SoftwareSerial mySerial(3, 2); //HC-05 Tx & Rx is connected to Arduino #3 & #2
const int xPin = A0;
// const int zPin = A1;

void setup()
{
  //Begin serial communication with Arduino and Arduino IDE (Serial Monitor)
  Serial.begin(9600);
  
  //Begin serial communication with Arduino and HC-05
  mySerial.begin(9600);

  Serial.println("Initializing...");
  Serial.println("The device started, waiting to pair it with bluetooth!");
}

void loop()
{
  int xValue = analogRead(xPin);
  xValue = map(xValue, 0, 1023, -256, 255);
  // int zValue = analogRead(zPin);
  String result = String(xValue) /*+ ", z = " + String(zValue) */+ "\n";
  Serial.print("x = ");
  Serial.print(result);
  mySerial.write(result.c_str());
  int command;
  if(Serial.available()) 
  {
    // command = ;
    // Serial.print("Command = ");
    // Serial.println(command);
    mySerial.write(Serial.read());//Forward what Serial received to Software Serial Port
  }
  if(mySerial.available()) 
  {
    command = mySerial.read();
    command -= 48;
    if(command == 0)
    {
      Serial.print("Vehilce off: ");
    }
    else if(command == 1)
    {
      Serial.print("Moving Forward: ");
    }
    else if(command == 2)
    {
      Serial.print("Moving Backward: ");
    }
    else if(command == 4)
    {
      Serial.print("Error 404\n");
    }
    else
    {
      Serial.print(command);
      Serial.print(": Unknown Command\n");
    }
    // Serial.write();//Forward what Software Serial received to Serial Port
  }
  delay(500);
}