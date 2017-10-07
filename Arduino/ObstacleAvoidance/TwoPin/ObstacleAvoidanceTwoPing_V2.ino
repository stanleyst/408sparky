// Left motor
int InA1 = 2;
int InB1 = 4;
int PWM1 = 3;  //PWM1 connects to pin 3
int PWM1_val = 50; //(25% = 64; 50% = 127; 75% = 191; 100% = 255)

// Right motor
int InA2 = 7;
int InB2 = 8;
int PWM2 = 6;  //PWM2 connects to pin 6
int PWM2_val = 72; //(25% = 64; 50% = 127; 75% = 191; 100% = 255)

int tooClose = 25; // distance to stop and decide what to do

// left 4-pin ping sensor
int trigPin = 11;    //Trig - green Jumper
int echoPin = 12;    //Echo - yellow Jumper

const int pingPin = 10; // for right 3-pin ping sensor
long durationLeft = 0, cmLeft = 0, inchesLeft = 0;
long durationRight = 0, inchesRight = 0, cmRight = 0;

long microsecondsToInches(long microseconds);
long microsecondsToCentimeters(long microseconds);
void rightBackward();
void rightForward();
void rightStop();
void leftForward();
void leftBackward();
void leftStop();
void spinClockwise();
void spinCounterClockwise();
void bothBackward();
void bothForward();
void bothStop();
void TurnLeft();
void TurnRight();
void Turn180();
void serialEvent();

void setup() {
  Serial.begin(9600);
  Serial.write('1');
  pinMode(InA2, OUTPUT);
  pinMode(InB2, OUTPUT);
  pinMode(PWM2, OUTPUT);
  pinMode(InA1, OUTPUT);
  pinMode(InB1, OUTPUT);
  pinMode(PWM1, OUTPUT); 
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

/*


*/

void loop() {

  // right 4 pin ping sensor code
  // The sensor is triggered by a HIGH pulse of 10 or more microseconds.
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
  digitalWrite(trigPin, LOW);
  delayMicroseconds(5);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
 
  // Read the signal from the sensor: a HIGH pulse whose
  // duration is the time (in microseconds) from the sending
  // of the ping to the reception of its echo off of an object.
  pinMode(echoPin, INPUT);
  durationLeft = pulseIn(echoPin, HIGH);

  // convert the time into a distance
 // inchesLeft = microsecondsToInches(durationLeft);
  cmLeft = microsecondsToCentimeters(durationLeft);

  // left 3 pin ping sensor code
  pinMode(pingPin, OUTPUT);
  digitalWrite(pingPin, LOW);
  delayMicroseconds(2);
  digitalWrite(pingPin, HIGH);
  delayMicroseconds(5);
  digitalWrite(pingPin, LOW);

  // The same pin is used to read the signal from the PING))): a HIGH pulse
  // whose duration is the time (in microseconds) from the sending of the ping
  // to the reception of its echo off of an object.
  pinMode(pingPin, INPUT);
  durationRight = pulseIn(pingPin, HIGH);

  // convert the time into a distance
//  inchesRight = microsecondsToInches(durationRight);
  cmRight = microsecondsToCentimeters(durationRight);

/*
int inChar;

if (inChar == 'S') // go straight
    {do {
        bothForward1();
    } while((cmLeft != tooClose) && (cmRight != tooClose));
    
    bothStop1();
    inChar = '5';
}
else if (inChar == 'R') // go right
  {
    TurnRight1();
    inChar = '5';
  }
else if (inChar == 'L') // go left 
{
    TurnLeft1();
    inChar = '5';
    }
else if (inChar == 'E') // error, can't find image
{
  spinClockwise1();
  delay(1000);
  bothStop1();
  delay(250);
  spinCounterclockwise1();
  delay(1000);
  bothStop1();
  inChar = '5';
  }
  
  */
  if((cmLeft < tooClose) && (cmRight < tooClose))
  {
    //Turn 180 degrees (approximately)
    Turn180();
  }
  // turn right
  else if(cmLeft < tooClose)
  {
      TurnRight();
  }
  // turn left
  else if(cmRight < tooClose)
  {
    TurnLeft();
  }
  else
  {
    bothForward();
  }

 // bothForward();

  delay(100);  
  
  
/*
  Serial.print(inchesLeft);
  Serial.print("in, ");
  Serial.print(cmLeft);
  Serial.print("cmLeft");
  Serial.println();

  */
}

// Motor control function definitions
void Turn180() {
    bothStop();
    delay(1000);
    spinClockwise();
    delay(600);
    bothStop(); 
}

void TurnRight() {
    bothStop();
    delay(1000);
    spinClockwise();
    delay(250);
    bothStop();
}

void TurnLeft() {
    bothStop();
    delay(1000);
    spinCounterClockwise();
    delay(250);
    bothStop();  
}

void rightBackward() {
  digitalWrite(InA2, HIGH);
  digitalWrite(InB2, LOW);
  analogWrite(PWM2, PWM2_val);
}

void rightForward() {
  digitalWrite(InA2, LOW);
  digitalWrite(InB2, HIGH);
  analogWrite(PWM2, PWM2_val);
}

void rightStop() {
  digitalWrite(InA2, LOW);
  digitalWrite(InB2, LOW);
  analogWrite(PWM2, 0);
}

void leftForward() {
  digitalWrite(InA1, HIGH);
  digitalWrite(InB1, LOW);
  analogWrite(PWM1, PWM1_val);
}

void leftBackward() {
  digitalWrite(InA1, LOW);
  digitalWrite(InB1, HIGH);
  analogWrite(PWM1, PWM1_val);
}

void leftStop() {
  digitalWrite(InA1, LOW);
  digitalWrite(InB1, LOW);
  analogWrite(PWM1, 0l);
}

void bothStop() {
  rightStop();
  leftStop();
}

void bothForward() {
  leftForward();
  rightForward();
}

void bothBackward() {
  leftBackward();
  rightBackward();
}

void spinCounterClockwise(){
  rightForward();
  leftBackward();
}

void spinClockwise(){
  rightBackward();
  leftForward();
}

void serialEvent() {
  // while (Serial.available())
  if (Serial.available()) 
  {
    // get the new byte:
    inChar = (char)Serial.read();
    // add it to the inputString:
    // inputString += inChar;
    // if the incoming character is a newline, set a flag so the main loop can
    // do something about it:
    // if (inChar == '\n') {
    //   stringComplete = true;
    // }
  }
}

// Ping function definitions

long microsecondsToInches(long microseconds) {
  // According to Parallax's datasheet for the PING))), there are 73.746
  // microseconds per inch (i.e. sound travels at 1130 feet per second).
  // This gives the distance travelled by the ping, outbound and return,
  // so we divide by 2 to get the distance of the obstacle.
  // See: http://www.parallax.com/dl/docs/prod/acc/28015-PING-v1.3.pdf
  return microseconds / 74 / 2;
}

long microsecondsToCentimeters(long microseconds) {
  // The speed of sound is 340 m/s or 29 microseconds per centimeter.
  // The ping travels out and back, so to find the distance of the object we
  // take half of the distance travelled.
  return microseconds / 29 / 2;
}


/* 
Capture image from a camera and write to a file

cam = cv2.VideoCapture(1)
s, im = cam.read() # captures image
cv2.imshow("Test Picture", im) # displays captured image
cv2.imwrite("test.bmp",im) # writes image test.bmp to disk
*/


