// -*- mode: c -*-
/*
   This is a test sketch for the Adafruit assembled Motor Shield for Arduino v2
   It won't work with v1.x motor shields! Only for the v2's with built in PWM
   control

   For use with the Adafruit Motor Shield v2
   ---->        http://www.adafruit.com/products/1438
*/


#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_MS_PWMServoDriver.h"

// Create the motor shield object with the default I2C address
Adafruit_MotorShield AFMS = Adafruit_MotorShield();

// Connect a stepper motor with 200 steps per revolution (1.8 degrees)
// to motor port #1 and #2 (M3 and M4)
Adafruit_StepperMotor *motorX = AFMS.getStepper(200, 1);
Adafruit_StepperMotor *motorY = AFMS.getStepper(200, 2);

int xPin = 2;
int yPin = 0;

void setup() {
    Serial.begin(9600);           // set up Serial library at 9600 bps
    Serial.println("Let's make pancakes!");

    AFMS.begin();  // create with the default frequency 1.6KHz, what does that mean?
    //AFMS.begin(1000);  // OR with a different frequency, say 1KHz
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
    // INTERLEAVE means that it alternates between single and double to get twice the resolution (but of course its half the speed).
    // MICROSTEP is a method where the coils are PWM'd to create smooth motion between steps.

    int stepStyle = DOUBLE;
    int fast = 800;  // steps/min of motor
    int slow = 100;
    bool motorRunning = false;
    
    motorX->setSpeed( slow );
    motorY->setSpeed( slow );

    // make motor move faster if joystick is pushed hard over
    if ((x > 1000) || (x < 10)) {
        motorX->setSpeed( fast );
    }
    if ((y > 1000) || (y < 10)) {
        motorY->setSpeed( fast );
    }

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
    
    /*
    Serial.println("Double coil steps");
    motor->step(100, FORWARD, DOUBLE);
    motor->step(100, BACKWARD, DOUBLE);

    Serial.println("Interleave coil steps");
    motor->step(100, FORWARD, INTERLEAVE);
    motor->step(100, BACKWARD, INTERLEAVE);

    Serial.println("Microstep steps");
    motor->step(50, FORWARD, MICROSTEP);
    motor->step(50, BACKWARD, MICROSTEP);
    */
}
