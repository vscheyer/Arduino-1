// -*- mode: c -*-
/*
 * Preprogrammed ontrol of two stepper motors.
 * AccelStepper allows us to program specific 2D points to move the motors to.
 * The motors start slow and accelerate to get to the destination as fast as possible, 
 * so the line travelled might not be straight...
 *
 * Driver for the Adafruit Motor Shield v2  http://www.adafruit.com/products/1438
 * API: https://learn.adafruit.com/adafruit-motor-shield-v2-for-arduino/library-reference#class-adafruit-steppermotor
 */

#include <AccelStepper.h>
#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_MS_PWMServoDriver.h"

// Create the motor shield object (more than one can be used, we only have one)
Adafruit_MotorShield AFMS = Adafruit_MotorShield();

// Create stepper motor objects, our motor does 200 steps per revolution (1.8 degrees)
// One motor is on port #1, the other #2 (M3 and M4)
Adafruit_StepperMotor* stepperX = AFMS.getStepper( 200, 1 );
Adafruit_StepperMotor* stepperY = AFMS.getStepper( 200, 2 );

int xPin = 2;  // analog pins for joystick
int yPin = 0;

//----------------------------------------
// callbacks to move the motors
//----------------------------------------
int stepStyle = DOUBLE;    // DOUBLE seems to be best combination of torque and speed.

void forwardX() {
    stepperX->onestep( FORWARD, stepStyle );
}
void backwardX() {
    stepperX->onestep( BACKWARD, stepStyle );
}
void forwardY() {
    stepperY->onestep( FORWARD, stepStyle );
}
void backwardY() {
    stepperY->onestep( BACKWARD, stepStyle );
}

// wrap the motors and movement functions in a magic object that
// calculates motor movements for us.

AccelStepper motorX( forwardX, backwardX );
AccelStepper motorY( forwardY, backwardY );


//----------------------------------------
void setup() {
    Serial.begin(9600);           // set up Serial library at 9600 bps
    Serial.println("Let's make pancakes!");

    // create motor library with the default frequency of 1600Hz for PWM,
    // I don't know what effect this has on the motor.
    AFMS.begin();

    motorX.setMaxSpeed( 400 );        // steps per second
    motorY.setMaxSpeed( 400 );
    motorX.setAcceleration( 100 );    // steps per second per second
    motorY.setAcceleration( 100 );

    // tell motors what to do.  Our world is probably about 2000x2000
    // This doesn't draw a straight line, it just gets there are fast as possible.
    
    motorX.moveTo( 400 );  // 100 steps, which is half a gear rotation
    motorY.moveTo( 200 );
}

//----------------------------------------
void loop() {
    bool xRunning = motorX.run();
    bool yRunning = motorY.run();

    if (!xRunning && !yRunning) {
        stepperX->release();   // let motor spin freely (don't lock gear in place)
        stepperY->release();   // keeps motor cooler when idle, too

        // we've reached our target, move to the next one.
        motorX.moveTo( 0 );
        motorY.moveTo( 0 );
    }
}
