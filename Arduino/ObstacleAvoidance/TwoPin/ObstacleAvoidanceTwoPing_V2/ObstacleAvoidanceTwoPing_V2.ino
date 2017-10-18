// Left motor
int InA1 = 2;
int InB1 = 4;
int PWM1 = 3;  //PWM1 connects to pin 3
int PWM1_val = 40; //(25% = 64; 50% = 127; 75% = 191; 100% = 255)

// Right motor
int InA2 = 7;
int InB2 = 8;
int PWM2 = 6;  //PWM2 connects to pin 6
int PWM2_val = 65; //(25% = 64; 50% = 127; 75% = 191; 100% = 255)

int tooClose = 35; // distance to stop and decide what to do

// left 4-pin ping sensor
// int trigPin = 11;    //Trig - green Jumper
// int echoPin = 12;    //Echo - yellow Jumper

const int pingPinRight = 10; // for right 3-pin ping sensor
const int pingPinLeft = 12; // for left 3-pin ping sensor
long durationLeft = 0, cmLeft = 0, inchesLeft = 0;
long durationRight = 0, inchesRight = 0, cmRight = 0;

// Serial Declarations
char inChar;

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
void spinClockwise1();
void spinCounterClockwise1();
void bothBackward1();
void bothForward1();
void bothStop1();
void TurnLeft1();
void TurnRight1();
void Turn1801();
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
  // pinMode(trigPin, OUTPUT);
  // pinMode(echoPin, INPUT);
}

/*


*/

void loop() {

  // left 4 pin ping sensor code
  // The sensor is triggered by a HIGH pulse of 10 or more microseconds.
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
//  digitalWrite(trigPin, LOW);
//  delayMicroseconds(5);
//  digitalWrite(trigPin, HIGH);
//  delayMicroseconds(10);
//  digitalWrite(trigPin, LOW);
 
  // Read the signal from the sensor: a HIGH pulse whose
  // duration is the time (in microseconds) from the sending
  // of the ping to the reception of its echo off of an object.
//  pinMode(echoPin, INPUT);
//  durationLeft = pulseIn(echoPin, HIGH);

  // convert the time into a distance
 // inchesLeft = microsecondsToInches(durationLeft);
 // cmLeft = microsecondsToCentimeters(durationLeft);

  // left 3 pin ping sensor code
  pinMode(pingPinLeft, OUTPUT);
  digitalWrite(pingPinLeft, LOW);
  delayMicroseconds(2);
  digitalWrite(pingPinLeft, HIGH);
  delayMicroseconds(5);
  digitalWrite(pingPinLeft, LOW);

  // The same pin is used to read the signal from the PING))): a HIGH pulse
  // whose duration is the time (in microseconds) from the sending of the ping
  // to the reception of its echo off of an object.
  pinMode(pingPinLeft, INPUT);
  durationLeft = pulseIn(pingPinLeft, HIGH);

  // convert the time into a distance
//  inchesRight = microsecondsToInches(durationRight);
  cmLeft = microsecondsToCentimeters(durationLeft);

  ///////////////////////

  // right 3 pin ping sensor code
  pinMode(pingPinRight, OUTPUT);
  digitalWrite(pingPinRight, LOW);
  delayMicroseconds(2);
  digitalWrite(pingPinRight, HIGH);
  delayMicroseconds(5);
  digitalWrite(pingPinRight, LOW);

  // The same pin is used to read the signal from the PING))): a HIGH pulse
  // whose duration is the time (in microseconds) from the sending of the ping
  // to the reception of its echo off of an object.
  pinMode(pingPinRight, INPUT);
  durationRight = pulseIn(pingPinRight, HIGH);

  // convert the time into a distance
//  inchesRight = microsecondsToInches(durationRight);
  cmRight = microsecondsToCentimeters(durationRight);

if (inChar == 'S') // go straight
    {
      if ((cmLeft != tooClose) && (cmRight != tooClose)) {
        bothForward();
    }
    
    bothStop();
    inChar = '5';
}
else if (inChar == 'R') // go right
  {
    if ((cmLeft != tooClose) && (cmRight != tooClose)) {
    TurnRight();
    } 
    inChar = '5';
  }
else if (inChar == 'L') // go left 
{
  if ((cmLeft != tooClose) && (cmRight != tooClose)) {
    TurnLeft();
  };
  inChar = '5';
    }

/*else // error, can't find image
{
  spinClockwise1();
  spinCounterClockwise1();
  delay(1000);
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

  delay(10);  
  
  
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
   // delay(1000);
    spinClockwise();
    delay(200);
    bothStop(); 
}

void TurnRight() {
    bothStop();
   // delay(1000);
    spinClockwise();
    delay(40);
    bothStop();
}

void TurnLeft() {
    bothStop();
   // delay(1000);
    spinCounterClockwise();
    delay(40);
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
  analogWrite(PWM1, 0);
}

void bothStop() {
  rightStop();
  leftStop();
}

void bothForward() {
  leftForward();
  rightForward();
  delay(100);
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


void TurnRight1() {
    spinClockwise1();
    delay(50);
}

void TurnLeft1() {
    spinCounterClockwise1();
    delay(50);
}

void leftForward1() {
  digitalWrite(InA1, HIGH);
  digitalWrite(InB1, LOW);
  analogWrite(PWM1, PWM1_val);
}

void rightForward1() {
  digitalWrite(InA2, LOW);
  digitalWrite(InB2, HIGH);
  analogWrite(PWM2, PWM2_val);
}

void leftBackward1() {
  digitalWrite(InA1, LOW);
  digitalWrite(InB1, HIGH);
  analogWrite(PWM1, (PWM1_val=30));
}

void rightBackward1() {
  digitalWrite(InA2, HIGH);
  digitalWrite(InB2, LOW);
  analogWrite(PWM2, (PWM2_val=64));
}


void bothStop1() {
  rightStop1();
  leftStop1();
}

void rightStop1() {
  digitalWrite(InA2, LOW);
  digitalWrite(InB2, LOW);
  analogWrite(PWM2, 0);
}

 void leftStop1() {
  digitalWrite(InA1, LOW);
  digitalWrite(InB1, LOW);
  analogWrite(PWM1, 0);
 }
  
void bothForward1() {
  leftForward1();
  rightForward1();
}

void spinCounterClockwise1(){
  rightForward1();
  leftBackward1();
}

void spinClockwise1(){
  rightBackward1();
  leftForward1();
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


