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

AccelStepper motorX( backwardX, forwardX );   // reverse so X is left to right on our robot
AccelStepper motorY( forwardY, backwardY );


//----------------------------------------
// Enable USB port and motors
//----------------------------------------
void setup() {
  Serial.begin( 9600 );           // set up Serial library at 9600 bps
  Serial.println("Let's make pancakes!");

  // create motor library with the default frequency of 1600Hz for PWM,
  // I don't know what effect this has on the motor.
  AFMS.begin();
}


//----------------------------------------
// tell motors what to do to draw the given list of points.
// This function takes over the Arduino until all points are reached by the motors.
//----------------------------------------
void drawShape( long points[][2], int numPoints ) {
  long* point = NULL;
  long* lastPoint = points[0];  // this is a pointer to where we were last so
                                // we can calculate distances to the next point
  float xDistance, yDistance, xSpeed, ySpeed;

  for (int i = 1; i < numPoints; i++) {
    point = points[i];

    Serial.print("Moving from (");
    Serial.print( lastPoint[0] ); Serial.print(","); Serial.print( lastPoint[1] );
    Serial.print(") to (");
    Serial.print( point[0] ); Serial.print(","); Serial.print( point[1] );
    Serial.println( ")" );

    // slow down or speed up one motor so line is straight and not a banana

    xDistance = point[0] - lastPoint[0];
    yDistance = point[1] - lastPoint[1];

    motorX.moveTo( point[0] );
    motorY.moveTo( point[1] );

    // max even speed is pretty much 60, otherwise motors get out of sync
    float limiter = abs( 60.0 / max( xDistance, yDistance ));

    // moveTo calculates the fastest speed, we want even 2D speed.
    motorX.setSpeed( xDistance * limiter );
    motorY.setSpeed( yDistance * limiter );

    Serial.print("Speed = "); Serial.print( motorX.speed() ); Serial.print(" x ");
    Serial.println( motorY.speed() );

    // motor.run() will move the motors the right amount and return false when they are done
    // runSpeed() is constant speed, run() does acceleration/deceleration

    long xD, yD;

    do {
      xD = motorX.distanceToGo();
      yD = motorY.distanceToGo();

      // Serial.print("To go = "); Serial.print( xD ); Serial.print(" "); Serial.println( yD );

      if (abs( xD ) > 0)
        motorX.runSpeedToPosition();

      if (abs( yD ) > 0)
        motorY.runSpeedToPosition();


      // loop until both motors are done running
    } while ((abs(xD) > 1) || (abs(yD) > 1));

    lastPoint = point;    // remember where we started to calculate future speeds.
  }
  Serial.println("Done!");
}

//----------------------------------------------------------------------
//----------------------------------------------------------------------
//----------------------------------------------------------------------

// Our physical world is probably about 2000x2000.

// Unlike most languages, C can't tell easily how long an array is,
// we must declare exactly how large the array is so the right amount of memory can be allocated
const int numPoints = 6;

// list of points for motors to visit
long star[numPoints][2] = {
  {0, 0},                       // must start at 0,0
  {124, 380},
  {248, 0},
  {76, 236},
  {324, 236},
  {0, 0}
};


// triangle
const int triPoints = 4;
long triangle[triPoints][2] = {
  {0, 0},
  {700, 200},
  {200, 700},
  {0, 0}
};

//----------------------------------------
void loop() {
  drawShape( star, numPoints );
  // drawShape( triangle, triPoints );

  // We're done! Let motor spin freely (don't lock gear in place)
  // keeps motor cooler when idle, too
  stepperX->release();
  stepperY->release();

  exit(0);  // don't keep looping, we could go into joystick mode here...  TODO

  // handleJoystick();
}
