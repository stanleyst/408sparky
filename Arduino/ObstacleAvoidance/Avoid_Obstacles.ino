// Right motor ??
int InA1 = 2;
int InB1 = 4;
int PWM1 = 3;  //PWM1 connects to pin 3
int PWM1_val = 64; //(25% = 64; 50% = 127; 75% = 191; 100% = 255)

// Left motor ??
int InA2 = 7;
int InB2 = 8;
int PWM2 = 6;  //PWM2 connects to pin 6
int PWM2_val = 64; //(25% = 64; 50% = 127; 75% = 191; 100% = 255)

const int pingPin = 9;

void setup() {
  Serial.begin(9600);
  pinMode(InA2, OUTPUT);
  pinMode(InB2, OUTPUT);
  pinMode(PWM2, OUTPUT);
  pinMode(InA1, OUTPUT);
  pinMode(InB1, OUTPUT);
  pinMode(PWM1, OUTPUT); 
}

void loop() {

  // establish variables for duration of the ping, and the distance result
  // in inches and centimeters:
  long duration, inches, cm;

  // The PING))) is triggered by a HIGH pulse of 2 or more microseconds.
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
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
  duration = pulseIn(pingPin, HIGH);

  // convert the time into a distance
  inches = microsecondsToInches(duration);
  cm = microsecondsToCentimeters(duration);

  if(cm < 20)
  {
    bothStop();
    delay(500);
    spinCounterClockwise;
    delay(500);
    rightStop();  
  }
  else
  {
    bothForward();
  }

  delay(100);  
}

// Motor control function definitions
void rightForward() {
  digitalWrite(InA2, HIGH);
  digitalWrite(InB2, LOW);
  analogWrite(PWM2, PWM2_val);
}

void rightBackward() {
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
  leftForward;
  rightForward;
}

void bothBackward() {
  leftBackward;
  rightBackward;
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


