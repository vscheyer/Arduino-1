/*
  Binary Counter  - 4 LEDs count 0 to 15

  Wire digital out 8,9,10,11 to leds (with 300ohm resistors)
  2016, David Whitney
*/

// Digital pins (on/off - high or low voltage only)
const int button1Pin = 2;  // pushbutton #1 pin
const int led1Pin = 8;     // first LED pin
const int soundPin = 12;   // buzzer

// Analog pins (any value)
const int speedPin = 0;   // analog potentiometer (0-1024)

/**
 * Configure the digital pins as input or output.
 * Enable the USB port to send debugging text.
 * This setup function runs _once_ when you press reset or power the board
 */
void setup() {
  
  // wait for serial port to connect. Needed for USB port.
  Serial.begin( 9600 );
  while (!Serial) {} 
  
  // initialize digital pins 8-11 as outputs to the LEDs.
  pinMode( led1Pin, OUTPUT);
  pinMode( led1Pin+1, OUTPUT);
  pinMode( led1Pin+2, OUTPUT);
  pinMode( led1Pin+3, OUTPUT);

  pinMode( soundPin, OUTPUT);

  // init our input pin connected to the button
  pinMode( button1Pin, INPUT);

  Serial.println("Starting");  
}


/**
 * Turn on an LED representing a bit (first led's pin# is 8)
 * @param bitPos - which bit (0-3) we are turning on
 */
void on( int bitPos ) {
  digitalWrite( bitPos + led1Pin, HIGH );
  Serial.print("1");  
}

void off( int bitPos ) {
  digitalWrite( bitPos + led1Pin, LOW );
  Serial.print("0");  
}


/**
 * convert number into bits and turn on LEDs
 */
void display( int n ) {
  
  for (int b = 0; b < 4; b++) {
    if (n & 0x1) { 
      on( b );  
    } else {
      off( b );
    }
    n = n >> 1;
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
 * Main loop
 * Wait for button to be pressed, then count from 1 to 15 in binary
 */
void loop() {

  playNote('c', 80 );
  //playNote('g', 80 );
  
  Serial.println("Waiting for button to be pressed...");
  while (!buttonPressed( button1Pin )) {  }  // don't proceed until button

  playNote('g', 150 );

  for (int i=0; i <16; i++) {
    display( i );  

    // potentiometer will send a voltage between 0 and 5v which results in
    // (0-1024) on analog pin A0
    int speed = analogRead( speedPin );
    // Serial.println( sensorValue );  

    delay( speed );  // wait 0 to 1024 ms
  }

  display( 0 );  // 0 turns off all LEDs
  
  //exit(0);
}
