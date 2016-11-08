/*
  Blink
  Turns on an LED on for one second, then off for one second, repeatedly.

  Most Arduinos have an on-board LED you can control attached to digital pin 13
  so you an wire an LED to pin 13, or just watch the built-in LED on the board.
   
  See the documentation at http://www.arduino.cc
 */


// the setup function runs once when you press reset or power the board
void setup() {
  pinMode(13, OUTPUT);  // initialize digital pin 13 as an output.
}


// the loop function runs over and over again forever
void loop() {
  
  // fake morse code by blinking short (100ms) or long (400ms) randomly
  // rand() can return a very large number, so just check even or odd using modulus (%)
  
  int flashLength = 100;
  if ((rand() % 2) > 0) {    // http://man7.org/linux/man-pages/man3/rand.3.html
    flashLength = 400;
  }
  
  digitalWrite( 13, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay( flashLength );              // wait for a second
  
  digitalWrite(13, LOW);    // turn the LED off by making the voltage LOW
  delay( flashLength );              // wait for a second
}
