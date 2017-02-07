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
// Or, create it with a different I2C address (say for stacking)
// Adafruit_MotorShield AFMS = Adafruit_MotorShield(0x61);

// Connect a stepper motor with 200 steps per revolution (1.8 degree)
// to motor port #1 and #2 (M3 and M4)
Adafruit_StepperMotor *myMotorX = AFMS.getStepper(200, 1);
Adafruit_StepperMotor *myMotorY = AFMS.getStepper(200, 2);

int yPin = 0;
int xPin = 1;

void setup() {
    Serial.begin(9600);           // set up Serial library at 9600 bps
    Serial.println("Stepper test!");

    AFMS.begin();  // create with the default frequency 1.6KHz, what does that mean?
    //AFMS.begin(1000);  // OR with a different frequency, say 1KHz

    myMotor->setSpeed(50);  // 10 rpm
}

void loop() {
    int x = analogRead( xPin );
    int y = analogRead( yPin );

    if (x > 800) {
        myMotorX->step(100, FORWARD, INTERLEAVE);
    } else if (x > 1000) {
        myMotorX->step(200, FORWARD, INTERLEAVE);
    }
    if (x < 300) {
        myMotorX->step(100, BACKWARD, INTERLEAVE);
    } else if (x < 100) {
        myMotorX->step(200, BACKWARD, INTERLEAVE);
    }

    if (y > 800) {
        myMotorY->step(100, FORWARD, INTERLEAVE);
    } else if (y > 1000) {
        myMotorY->step(200, FORWARD, INTERLEAVE);
    }
    if (y < 300) {
        myMotorY->step(100, BACKWARD, INTERLEAVE);
    } else if (y < 100) {
        myMotorY->step(200, BACKWARD, INTERLEAVE);
    }

    /*Serial.println("Double coil steps");
      myMotor->step(100, FORWARD, DOUBLE);
      myMotor->step(100, BACKWARD, DOUBLE);

      Serial.println("Interleave coil steps");
      myMotor->step(100, FORWARD, INTERLEAVE);
      myMotor->step(100, BACKWARD, INTERLEAVE);

      Serial.println("Microstep steps");
      myMotor->step(50, FORWARD, MICROSTEP);
      myMotor->step(50, BACKWARD, MICROSTEP);*/
}
