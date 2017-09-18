// Right motor
int InA1 = 2;
int InB1 = 4;
int PWM1 = 3;  //PWM1 connects to pin 3
int PWM1_val = 127; //(25% = 64; 50% = 127; 75% = 191; 100% = 255)

// Left motor
int InA2 = 7;
int InB2 = 8;
int PWM2 = 6;  //PWM2 connects to pin 6
int PWM2_val = 127; //(25% = 64; 50% = 127; 75% = 191; 100% = 255)
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
    rightForward();
    leftForward();
    delay(5000);
    rightBackward();
    delay(1000);
    leftBackward();
    delay(5000);
    rightStop();
    leftStop();
    delay(5000);
}

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

