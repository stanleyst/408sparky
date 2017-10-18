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

int tooClose = 30; // distance to stop and decide what to do
char inChar;

int searchCount = 0; // variable to determine whether to spin or go forward
                    // if no object is found

// left 4-pin ping sensor
const int PingPinFront = 11;    // Front 3-pin ping sensor
const int PingPinLeft = 12;    // Left 3-pin ping sensor
const int PingPinRight = 10; // Right 3-pin ping sensor
long durationLeft = 0, cmLeft = 0;
long durationRight = 0, cmRight = 0;
long durationFront = 0, cmFront = 0;

long microsecondsToInches(long microseconds);
long microsecondsToCentimeters(long microseconds);
void rightBackward();
void rightForward();
void rightStop();
void leftForward();
void leftBackward();
void leftStop();
void spinClockwise(int delayTime);
void spinCounterClockwise(int delayTime);
void bothForward(int delayTime);
void bothBackward(int delayTime);
void bothStop(int delayTime);
void TurnLeft(int delayTime);
void TurnRight(int delayTime);
void Turn180();
void KangaSlowRight(int delayTime);
void KangaSlowLeft(int delayTime);
void KangaR_Backward();
void KangaL_Backward();
void bothStop(int delayTime);
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
}

/*
*/

void loop() {

  // left 4 pin ping sensor code
  pinMode(PingPinLeft, OUTPUT);
  digitalWrite(PingPinLeft, LOW);
  delayMicroseconds(2);
  digitalWrite(PingPinLeft, HIGH);
  delayMicroseconds(5);
  digitalWrite(PingPinLeft, LOW);
 
  // Read the signal from the sensor: a HIGH pulse whose
  // duration is the time (in microseconds) from the sending
  // of the ping to the reception of its echo off of an object.
  pinMode(PingPinLeft, INPUT);
  durationLeft = pulseIn(PingPinLeft, HIGH);

  // convert the time into a distance
 // inchesLeft = microsecondsToInches(durationLeft);
  cmLeft = microsecondsToCentimeters(durationLeft);

  // right 3 pin ping sensor code
  pinMode(PingPinRight, OUTPUT);
  digitalWrite(PingPinRight, LOW);
  delayMicroseconds(2);
  digitalWrite(PingPinRight, HIGH);
  delayMicroseconds(5);
  digitalWrite(PingPinRight, LOW);

  // The same pin is used to read the signal from the PING))): a HIGH pulse
  // whose duration is the time (in microseconds) from the sending of the ping
  // to the reception of its echo off of an object.
  pinMode(PingPinRight, INPUT);
  durationRight = pulseIn(PingPinRight, HIGH);

  // convert the time into a distance
//  inchesRight = microsecondsToInches(durationRight);
  cmRight = microsecondsToCentimeters(durationRight);

 // front 3 pin ping sensor code
  pinMode(PingPinFront, OUTPUT);
  digitalWrite(PingPinFront, LOW);
  delayMicroseconds(2);
  digitalWrite(PingPinFront, HIGH);
  delayMicroseconds(5);
  digitalWrite(PingPinFront, LOW);

  pinMode(PingPinFront, INPUT);
  durationFront = pulseIn(PingPinFront, HIGH);
  cmFront = microsecondsToCentimeters(durationFront);



if (inChar == 'S'){ // go straight
    if ((cmLeft != tooClose) && (cmRight != tooClose) && (cmFront != tooClose))
    {
      bothForward(80);
    }
    else 
    {bothStop(1);
    inChar = '5';
    }
    searchCount = 0;
}
else if (inChar == 'R') // go right
  {
    KangaSlowRight(40);
    inChar = '5';
    searchCount = 0;
  }
else if (inChar == 'L') // go left 
{
    KangaSlowLeft(40);
    inChar = '5';
    searchCount = 0;
}

//else 
//  {
//    spinClockwise();
//    delay(1000);
//  }


    
 /* 
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
*/

  if (cmFront < tooClose)
  {
    if ((cmRight < tooClose) && (cmLeft < tooClose))
    {
      Turn180();
    }
    else if (cmLeft < tooClose)
    {
      TurnRight(200);
    }
    else if (cmRight < tooClose)
    {
      TurnLeft(200);
    }
    else
    {
      TurnRight(200);
    }
  }
  else if (cmRight < tooClose)
  {
    TurnLeft(100);
  }
  else if (cmLeft < tooClose)
  {
    TurnRight(100);
  }
  // if it can't find the object AND there are no obstacles,
  // go forward slightly for a bit in hopes that the object is
  // still in front of Sparky, and the image processing code
  // just didn't send a command in time. After a set time,
  // give up and start spinning to search  
  else
  {
    if(searchCount < 5)
    {
      bothForward(20);
      bothStop(100);
      searchCount++;
    }
    else
    {
      TurnRight(30);
      bothStop(100);
    }
  }
     
      


 // delay(100);
  
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
    bothStop(1);
   // delay(10);
    spinClockwise(600);
   // delay(600);
    bothStop(1); 
}

void TurnRight(int delayTime) {
   // bothStop();
   // delay(1);
    spinClockwise(delayTime);
   // delay(delayTime);
    bothStop(1);
   // delay(1);
}

void TurnLeft(int delayTime) {
    //bothStop();
   // delay(1);
    spinCounterClockwise(delayTime);
    //delay(delayTime);
    bothStop(1);
   // delay(1);  
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

void bothStop(int delayTime) {
  rightStop();
  leftStop();
  delay(delayTime);
}

void bothForward(int delayTime) {
  leftForward();
  rightForward();
  delay(delayTime);
}

void bothBackward(int delayTime) {
  leftBackward();
  rightBackward();
  delay(delayTime);
}

void spinCounterClockwise(int delayTime){
  rightForward();
  leftBackward();
  delay(delayTime);
}

void spinClockwise(int delayTime){
  rightBackward();
  leftForward();
  delay(delayTime);
}

void KangaL_Backward() {
  digitalWrite(InA1, LOW);
  digitalWrite(InB1, HIGH);
  analogWrite(PWM1, 30);
}

void KangaR_Backward() {
  digitalWrite(InA2, HIGH);
  digitalWrite(InB2, LOW);
  analogWrite(PWM2, 64);
}

void KangaSlowLeft(int delayTime){
  rightForward();
  KangaL_Backward();
  delay(delayTime);
}

void KangaSlowRight(int delayTime){
  KangaR_Backward();
  leftForward();
  delay(delayTime);
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


