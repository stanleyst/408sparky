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

void setup() {
  Serial.begin(9600);
  pinMode(InA2, OUTPUT);
  pinMode(InB2, OUTPUT);
  pinMode(PWM2, OUTPUT);
  pinMode(InA1, OUTPUT);
  pinMode(InB1, OUTPUT);
  pinMode(PWM1, OUTPUT); 
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

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

  if((cmLeft < tooClose) && (cmRight < tooClose))
  {
    //Turn 180 degrees (approximately)
    bothStop();
    delay(1000);
    spinClockwise();
    delay(600);
    bothStop();  
  }
  // turn right
  else if(cmLeft < tooClose)
  {
    bothStop();
    delay(1000);
    spinClockwise();
    delay(250);
    bothStop();  
  }
  // turn left
  else if(cmRight < tooClose)
  {
    bothStop();
    delay(1000);
    spinCounterClockwise();
    delay(250);
    bothStop();  
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

int KangaYN; // variable to determine if the object is present.
int KangaRL; // variable to determine if the object is in the left or right half of the frame, and how much.
int KangaUD; // variable to determine if the object is in the upper or lower half of the frame, and how much.

if KangaYN = 1; // yes, the object is present
   if KangaRL = 1; // the object is on the right side of the picture
      if KangaUD = 1; // the object is on the upper half of the picture
          spinClockwise();
      else if KangaUD = 0;
   else if KangaRL = 0;
      if KangaUD = 1;
      else if KangaUD = 0;
      
else if KangaYN = 0;   




*/


