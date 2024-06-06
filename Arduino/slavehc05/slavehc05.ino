#include <SoftwareSerial.h>
int enA = 9;
int enB = 5;
int in1 = 8;
int in2 = 7;
int in3 = 3;
int in4 = 4;
SoftwareSerial BTSerial(10, 11); // RX | TX

const byte numChars = 32;
char receivedChars[numChars];   // an array to store the received data

boolean newData = false;

void recvWithEndMarker() {
    static byte ndx = 0;
    char endMarker = '\n';
    char rc;
    
    while (BTSerial.available() > 0 && newData == false) {
        rc = BTSerial.read();

        if (rc != endMarker) {
            receivedChars[ndx] = rc;
            ndx++;
            if (ndx >= numChars) {
                ndx = numChars - 1;
            }
        }
        else {
            receivedChars[ndx] = '\0'; // terminate the string
            ndx = 0;
            newData = true;
        }
    }
}

int showNewData() {
  int intValue = 0;
    if (newData == true) {
        // Serial.print("This just in ... ");
        intValue = atoi(receivedChars);
        // Serial.println(intValue);
        newData = false;
    }
  return intValue;
}

void setup()
{
  // pinMode(9, OUTPUT);  // this pin will pull the HC-05 pin 34 (key pin) HIGH to switch module to AT mode
  // digitalWrite(9, HIGH);
  // Serial.begin(38400);
  Serial.begin(9600);
  BTSerial.begin(9600);  // HC-05 default speed in AT command more
	// Set all the motor control pins to outputs
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(in1, OUTPUT);
	pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
	pinMode(in4, OUTPUT);
  // Turn off motors - Initial state
	digitalWrite(in1, LOW);
	digitalWrite(in2, LOW);
	digitalWrite(in3, LOW);
	digitalWrite(in4, LOW);
  Serial.println("Initializing...");
  Serial.println("The device started, waiting to recieve instructions!");
}

void loop()
{
  // Keep reading from HC-05 and send to Arduino Serial Monitor
  int command;
  if (BTSerial.available())
  {
    recvWithEndMarker();
    command = showNewData();
    Serial.println(command);
    //Serial.write(command.c_str());
    if (command >= -16 && command < 16)
    {
      //off
      Serial.print("Command = ");
      digitalWrite(in1, LOW);
    	digitalWrite(in2, LOW);
      digitalWrite(in3, LOW);
    	digitalWrite(in4, LOW);
      BTSerial.write("0");
    }
    else if (command >= 16 && command < 256)
    {
      //forward
      Serial.print("Command = ");
	    analogWrite(enA, command);
	    analogWrite(enB, command);
      digitalWrite(in1, HIGH);
  	  digitalWrite(in2, LOW);
      digitalWrite(in3, HIGH);
  	  digitalWrite(in4, LOW);
      BTSerial.write("1");
    }
    else if (command >= -256 && command < -16)
    {
      //backward
      Serial.print("Command = ");
      command = command + 1;
      command *= -1;
	    analogWrite(enA, command);
      analogWrite(enB, command);
      digitalWrite(in1, LOW);
    	digitalWrite(in2, HIGH);
      digitalWrite(in3, LOW);
    	digitalWrite(in4, HIGH);
      BTSerial.write("2");
    }
    else
    {
      Serial.print("Command = ");
      BTSerial.write("4");
    }
  }

  // Keep reading from Arduino Serial Monitor and send to HC-05
  if (Serial.available())
  {
    BTSerial.write(Serial.read());
  }
  delay(500);
}


// Enter AT commands:
// OK
// OK
// +NAME:SlaveHC05
// OK
// OK
// +ROLE:0
// OK
// +ADDR:0:13:102D53
// OK

