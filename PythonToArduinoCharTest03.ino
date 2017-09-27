/*
  Serial Event example

  When new serial data arrives, this sketch adds it to a String.
  When a newline is received, the loop prints the string and clears it.

  A good test for this is to try it with a GPS receiver that sends out
  NMEA 0183 sentences.

  NOTE: The serialEvent() feature is not available on the Leonardo, Micro, or
  other ATmega32U4 based boards.

  created 9 May 2011
  by Tom Igoe

  This example code is in the public domain.

  http://www.arduino.cc/en/Tutorial/SerialEvent
*/

String inputString = "";         // a String to hold incoming data
// boolean stringComplete = false;  // whether the string is complete
// boolean bCompletedString = false;
// String testString = "";
char inChar = '5'; // just set it to a random value we won't use
int endProgram = 0;

void setup() {
  // initialize serial:
  Serial.begin(9600);
  // reserve 200 bytes for the inputString:
  // inputString.reserve(200);
  Serial.write('1'); // tell Python we are ready
}

void loop() {
  // print the string when a newline arrives:
  if (inChar == 'L') 
  {
    // Serial.write('0'); // tell python to close serial
    // Serial.read();     // Serial.flush();
    delay(100);
    // Serial.flush();

    Serial.print((char)inChar);

    delay(100);

    inputString += inChar;
    inChar = '5';
    endProgram++;
  }

  else if (inChar == 'S') 
  {
    // Serial.write('0'); // tell python to close serial
    // Serial.read();     // Serial.flush();
    delay(100);
    // Serial.flush();

    Serial.print((char)inChar);

    delay(100);

    inputString += inChar;
    inChar = '5';
    endProgram++;

  }

  else if (inChar == 'R') 
  {
    // Serial.write('0'); // tell python to close serial
    // Serial.read();     // Serial.flush();
    delay(100);
    // Serial.flush();

    Serial.print((char)inChar);

    delay(100);

    inputString += inChar;
    inChar = '5';
    endProgram++;
  }

  if(endProgram >= 3)
  {
    Serial.write('0');
    endProgram = 0;
    delay(100);
    Serial.print(inputString);
  }    
}

/*
  SerialEvent occurs whenever a new data comes in the hardware serial RX. This
  routine is run between each time loop() runs, so using delay inside loop can
  delay response. Multiple bytes of data may be available.
*/
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