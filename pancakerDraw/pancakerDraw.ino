// -*- mode: c++ -*-
/*
 * Preprogrammed ontrol of two stepper motors.
 * AccelStepper allows us to program specific 2D points to move the motors to.
 * The motors start slow and accelerate to get to the destination as fast as possible,
 * so the line travelled might not be straight...
 *
 * Driver for the Adafruit Motor Shield v2  http://www.adafruit.com/products/1438
 * API for Motor: https://learn.adafruit.com/adafruit-motor-shield-v2-for-arduino/library-reference#class-adafruit-steppermotor
 * API for AccelStepper: http://www.airspayce.com/mikem/arduino/AccelStepper/classAccelStepper.html
 */

#include <AccelStepper.h>
#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_MS_PWMServoDriver.h"

#include <stdio.h>
#include <stdlib.h>  // for math functions like abs()
// #include <list>   *sigh* lists are C++ only, we are stuck with arrays on Arduino

// Create the motor shield object (more than one can be used, we only have one)
Adafruit_MotorShield AFMS = Adafruit_MotorShield();

// Create stepper motor objects, our motor does 200 steps per revolution (1.8 degrees)
// One motor is on port #1, the other #2 (M3 and M4)
Adafruit_StepperMotor* stepperX = AFMS.getStepper( 200, 1 );
Adafruit_StepperMotor* stepperY = AFMS.getStepper( 200, 2 );

int xPin = 2;  // analog pins for joystick
int yPin = 0;

//----------------------------------------
// functions to move the motors
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
// calculates motor movements and speeds for us.

AccelStepper motorX( forwardX, backwardX );
AccelStepper motorY( forwardY, backwardY );


//----------------------------------------
// Enable USB port and motors
//----------------------------------------
void setup() {
  Serial.begin(9600);           // set up Serial library at 9600 bps
  Serial.println("Let's make pancakes!");

  // create motor library with the default frequency of 1600Hz for PWM,
  // I don't know what effect this has on the motor.
  AFMS.begin();

  motorX.setSpeed( 100 );        // steps per second
  motorY.setSpeed( 100 );

  // motorX.setMaxSpeed( 400 );        // steps per second
  // motorY.setMaxSpeed( 400 );
  // motorX.setAcceleration( 100 );    // steps per second per second
  // motorY.setAcceleration( 100 );
}


//----------------------------------------
// tell motors what to do.  Our physical world is probably about 2000x2000
// This doesn't draw a straight line, it just gets there are fast as possible.
//----------------------------------------

// Global variables - these are remembered throughout the program (as
// opposed to the local function variables).

// list of points for motors to visit

const int numPoints = 6;  // Unlike most languages, C can't tell easily how long an array is

int point[numPoints][2] = {
  {0, 0},
  {124, 380},
  {248, 0},
  {76, 236},
  {324, 236},
  {0, 0}
};
int i = 0;              // which point we are on

int* lastPoint = NULL;  // this is a pointer to where we were last so
                        // we can calculate distances to the next point

//----------------------------------------
void loop() {
  int* nextPoint = NULL;
  int xDistance, yDistance;

  // motor.run() will move the motors the right amount and return false when they are done
  while (motorX.run() && motorY.run()) {
    // wait until motors are done
  }

  // We've reached our target, move to the next one.
  if (++i < numPoints) {
    nextPoint = point[i];

    if (lastPoint != NULL) {
      // slow down or speed up one motor so line is straight and not a banana

      xDistance = abs( nextPoint[0] - lastPoint[0]);
      yDistance = abs( nextPoint[1] - lastPoint[1] );

      motorX.setSpeed( 100 );        // steps per second
      motorY.setSpeed( 100 * xDistance/yDistance );
    }

    motorX.moveTo( nextPoint[0] );
    motorY.moveTo( nextPoint[1] );

    lastPoint = nextPoint;    // remember where we started to calculate future speeds.

  } else {
    // We're done! Let motor spin freely (don't lock gear in place)
    // keeps motor cooler when idle, too

    stepperX->release();
    stepperY->release();
  }
}
