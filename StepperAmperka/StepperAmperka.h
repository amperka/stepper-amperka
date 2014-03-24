/*
  StepperAmperka.h - Fork of stepper library for Wiring/Arduino to
        make it compatible out of the box with Amperka Motor Shield
  
  Original library     (0.1) by Tom Igoe.
  Two-wire modifications   (0.2) by Sebastian Gassner
  Combination version   (0.3) by Tom Igoe and David Mellis
  Bug fix for four-wire   (0.4) by Tom Igoe, bug fix from Noah Shibley
  Half step mode        (0.4a) by Vasily Basalaev

*/

#ifndef ARDUINO
#error "Arduino version not defined"
#endif

#if ARDUINO >= 100
#include <Arduino.h>
#else
#include <WProgram.h>
#endif

// ensure this library description is only included once
#ifndef StepperAmperka_h
#define StepperAmperka_h

enum STEP_TYPE {
    WAVE_DRIVE,
    FULL_STEP,
    HALF_STEP
};

// library interface description
class StepperAmperka {
  public:
    // constructor:
    StepperAmperka(
        int number_of_steps,
        uint8_t motor_pin_1 = 4,
        uint8_t motor_pin_2 = 5,
        uint8_t motor_pin_3 = 6,
        uint8_t motor_pin_4 = 7);

    // speed setter method:
    void setSpeed(long revs_per_minute);

    // mover method:
    void step(int number_of_steps, uint8_t step_type=FULL_STEP);

  protected:
    void stepMotor(int this_step, uint8_t step_type);
    
    bool direction;        // Direction of rotation
    int speed;             // Speed in RPMs
    unsigned long step_delay; // delay between steps, in ms, based on speed
    int number_of_steps;      // total number of steps this motor can take
    int step_number; // which step the motor is on
    
    // motor pin numbers:
    uint8_t motor_pin_1;
    uint8_t motor_pin_2;
    uint8_t motor_pin_3;
    uint8_t motor_pin_4;
    
    long last_step_time;      // time stamp in ms of when the last step was taken
};

#endif

