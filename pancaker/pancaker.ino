// -*- mode: c -*-
/*
 * Joystick control of two stepper motors
 *
 * Driver for the Adafruit Motor Shield v2  http://www.adafruit.com/products/1438
 * API: https://learn.adafruit.com/adafruit-motor-shield-v2-for-arduino/library-reference#class-adafruit-steppermotor
*/

#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_MS_PWMServoDriver.h"

// Create the motor shield object with the default I2C address
Adafruit_MotorShield AFMS = Adafruit_MotorShield();

// Connect stepper motors, our motor does 200 steps per revolution (1.8 degrees)
// Use motor port #1 and #2 (M3 and M4)
Adafruit_StepperMotor* motorX = AFMS.getStepper( 200, 1 );
Adafruit_StepperMotor* motorY = AFMS.getStepper( 200, 2 );

int xPin = 2;
int yPin = 0;

void setup() {
    Serial.begin(9600);           // set up Serial library at 9600 bps
    Serial.println("Let's make pancakes!");

    // create motor library with the default frequency of 1600Hz for PWM,
    // I don't know what effect this has on the motor.
    AFMS.begin();
}

void loop() {
    int x = analogRead( xPin );
    int y = analogRead( yPin );
    y = 1023 - y;  // flip it to match our machine

/*
    Serial.print("Joystick: ");
    Serial.print( x );
    Serial.print(", ");
    Serial.println( y );
*/

    // stepStyle is
    // SINGLE means single-coil activation,
    // DOUBLE means 2 coils are activated at once (for higher torque) and
    // INTERLEAVE means that it alternates between single and double to get twice the resolution (but of course it's half the speed).
    // MICROSTEP is a slow method where the coils are PWM'd to create smooth motion between steps.

    int stepStyle = DOUBLE;    // DOUBLE seems to be best combination of torque and speed.
    bool motorRunning = false;

/*
    int fast = 400;  // steps/revolution of motor
    int slow = 50;

    int xSpeed = slow;
    int ySpeed = slow;

    // make motor move faster if joystick is pushed hard over
    if ((x > 1000) || (x < 10)) {
        xSpeed = fast;
    }
    if ((y > 1000) || (y < 10)) {
        ySpeed = fast;
    }

    motorX->setSpeed( xSpeed );   // setSpeed is ignored is onestep() is used.
    motorY->setSpeed( ySpeed );
*/

    if (x > 750) {
        motorX->onestep( FORWARD, stepStyle );
        motorRunning = true;
    }
    if (x < 250) {
        motorX->onestep( BACKWARD, stepStyle);
        motorRunning = true;
    }

    if (y > 750) {
        motorY->onestep( FORWARD, stepStyle );
        motorRunning = true;
    }
    if (y < 250) {
        motorY->onestep( BACKWARD, stepStyle);
        motorRunning = true;
    }

    if (!motorRunning) {
        motorX->release();   // let motor spin freely (don't lock gear in place)
        motorY->release();   // keeps motor cooler when idle, too
    }

}
