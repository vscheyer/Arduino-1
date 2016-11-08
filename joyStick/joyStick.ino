/*
  Kitchen sink app that makes noise, blinks LEDs in binary counter fashion,
  Reads a potentiometer to set speed of blinking
  Reads a joystick to set speed of a simple motor.

  Basically this is all the SparkFun Starter Kit tutorials rolled into one.
  https://www.vilros.com/uskcode
  
  Wire digital out 8,9,10,11 to LEDs (with 300ohm resistors)
  2016, David Whitney
*/

// Digital pins (on/off - high or low voltage only)
const int button1Pin = 2;  // pushbutton #1 pin
const int led1Pin = 8;     // first LED pin
const int soundPin = 12;   // buzzer   

// Analog pins (any value)
const int speedPin = 0;   // analog potentiometer [0-1023]
const int motorPin = 6;   // 

// JoyStick
const int joyButtonPin = 4; // digital
const int joyXPin = 4;      // analog [0-1023]
const int joyYPin = 5;      // analog [0-1023]

/**
 * Configure the digital pins as input or output.
 * Enable the USB port to send debugging text.
 * This setup function runs _once_ when you press reset or power the board
 */
void setup() {
  
  // wait for serial port to connect. Needed for USB port.
  Serial.begin( 9600 );
  while (!Serial) {} 

  // digital inputs need to be defined (analog do not)

  // initialize digital pins 8-11 as outputs to the LEDs.
  pinMode( led1Pin, OUTPUT);
  pinMode( led1Pin+1, OUTPUT);
  pinMode( led1Pin+2, OUTPUT);
  pinMode( led1Pin+3, OUTPUT);

  pinMode( soundPin, OUTPUT);
  pinMode( motorPin, OUTPUT);

  // button presses (LOW means pressed, ironically)
  pinMode( button1Pin, INPUT);
  
  pinMode( joyButtonPin, INPUT);
  digitalWrite( joyButtonPin, HIGH );  // what does this do? Button doesnt work without it
  
  
  Serial.println("Starting");  
}


/**
 * Turn on an LED representing a bit (first led's pin# is 8)
 * @param bitPos - which bit (0-3) we are turning on
 */
void ledOn( int bitPos ) {
  digitalWrite( bitPos + led1Pin, HIGH );
  Serial.print("1");  
}

void ledOff( int bitPos ) {
  digitalWrite( bitPos + led1Pin, LOW );
  Serial.print("0");  
}


/**
 * convert number into bits and turn on LEDs
 */
void display( int n ) {
  
  for (int b = 0; b < 4; b++) {
    if (n & 0x1) {   // check least significant bit
      ledOn( b );  
    } else {
      ledOff( b );
    }
    n = n >> 1;  // shift bits rights
  }
  Serial.println();  
}

/**
 * If the button is pressed, the curcuit will be grounded (LOW)
 * If the button is not pressed then the 10K resistor will show a HIGH voltage 
 * @return true if button is pressed 
 */
bool buttonPressed( int buttonPin ) {
  int buttonVoltage = digitalRead( buttonPin );
  // Serial.println( buttonVoltage );
  return (buttonVoltage == LOW);
}

// Motor goes from 0-255, but analog inputs are 0-1023
void setMotorSpeed( int speed ) {
  speed = speed / 4;
  analogWrite( motorPin, speed );
  //Serial.print("Motor speed = ");
  //Serial.println( speed );
  
}

/**
 * find the frequency for the letter note and play it.
 * @param note from middle 'c' to high 'C'
 * @param duration - ms to play note
 */
void playNote( char note, int duration ) {
  char names[] =      { 'c', 'd', 'e', 'f', 'g', 'a', 'b', 'C' };
  int frequencies[] = { 262, 294, 330, 349, 392, 440, 494, 523 };
  
  int frequency = 0;
  for (int i=0; i < 8; i++) {
    if (names[i] == note) {             // Is this the one?
      frequency = frequencies[i];       // Yes!
      break;
    }
  } 

  tone( soundPin, frequency, duration);
  delay( duration );
}


/**
 * Test out the joystick
 */
void handleJoystick() {
  Serial.print("Joystick button: ");
  Serial.println( buttonPressed( joyButtonPin )? "true" : "false"); 

  int x = analogRead( joyXPin );
  int y = analogRead( joyYPin );
  Serial.print( x );
  Serial.print(", ");
  Serial.println( y );

  
}

/**
 * Main loop
 * Wait for button to be pressed, then count from 1 to 15 in binary
 */
void loop() {

  handleJoystick();  // just prints joystick state
  //return;

  playNote('c', 80 );
  
  Serial.println("Waiting for button to be pressed...");
  while (!buttonPressed( button1Pin )) {  }  // don't proceed until button

  playNote('g', 150 );

  for (int i=0; i <16; i++) {
    display( i );  

    // potentiometer will send a voltage between 0 and 5v which results in
    // (0-1024) on analog pin A0
    int speed = analogRead( speedPin );
    // Serial.println( sensorValue );  

    setMotorSpeed( analogRead( joyXPin ) );

    delay( speed );  // wait 0 to 1024 ms
  }

  display( 0 );  // 0 turns off all LEDs
  setMotorSpeed( 0 );


  //exit(0);
}
