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
// const int speedPin = 0;   // analog potentiometer [0-1023]
// const int motorPin = 6;   // 

// JoyStick
const int joyButtonPin = 4; // digital
const int joyXPin = 2;      // analog [0-1023]
const int joyYPin = 3;      // analog [0-1023]

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
 * Test out the joystick
 */
void handleJoystick() {
 // Serial.print("Joystick button: ");
 // Serial.println( buttonPressed( joyButtonPin )? "true" : "false"); 

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
  return;


  for (int i=0; i < 4; i++) {
    ledOn( i );  

    // potentiometer will send a voltage between 0 and 5v which results in
    // (0-1024) on analog pin A0
    // int speed = analogRead( speedPin );
    // Serial.println( sensorValue );  

    int speed = 100;
    delay( speed );  // wait 0 to 1024 ms
  }

  delay( 400 );
  display(0);
  delay( 400 );

  //exit(0);
}
