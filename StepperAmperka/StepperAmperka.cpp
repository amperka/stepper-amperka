/*
  StepperAmperka.cpp - Fork of stepper library for Wiring/Arduino to
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
 
#include "StepperAmperka.h"


/*
 *   constructor for four-pin version
 *   Sets which wires should control the motor.
 */

StepperAmperka::StepperAmperka(
        int number_of_steps,
        unsigned char motor_pin_1,
        unsigned char motor_pin_2,
        unsigned char motor_pin_3,
        unsigned char motor_pin_4)
{
  this->step_number = 0;      // which step the motor is on
  this->speed = 0;        // the motor speed, in revolutions per minute
  this->direction = 0;      // motor direction
  this->last_step_time = 0;    // time stamp in ms of the last step taken
  this->number_of_steps = number_of_steps;    // total number of steps for this motor
  
  // Arduino pins for the motor control connection:
  this->motor_pin_1 = motor_pin_1;
  this->motor_pin_2 = motor_pin_2;
  this->motor_pin_3 = motor_pin_3;
  this->motor_pin_4 = motor_pin_4;

  // setup the pins on the microcontroller:
  pinMode(this->motor_pin_1, OUTPUT);
  pinMode(this->motor_pin_2, OUTPUT);
  pinMode(this->motor_pin_3, OUTPUT);
  pinMode(this->motor_pin_4, OUTPUT);
}

/*
  Sets the speed in revs per minute

*/
void StepperAmperka::setSpeed(long revs_per_minute)
{
  this->step_delay = 60L * 1000L / this->number_of_steps / revs_per_minute;
}

/*
  Moves the motor steps_to_move steps.  If the number is negative, 
   the motor moves in the reverse direction.
 */
void StepperAmperka::step(int steps_to_move, uint8_t step_type)
{  
  int steps_left = abs(steps_to_move);  // how many steps to take
  
  // determine direction based on whether steps_to_mode is + or -:
  if (steps_to_move > 0) {this->direction = 1;}
  if (steps_to_move < 0) {this->direction = 0;}

  if (step_type == FULL_STEP) {
    digitalWrite(this->motor_pin_2, HIGH);
    digitalWrite(this->motor_pin_3, HIGH);
  }

  unsigned long step_delay = this->step_delay;
  if (step_type == HALF_STEP)
    step_delay /= 2;
    
  // decrement the number of steps, moving one step each time:
  while(steps_left > 0) {

    // move only if the appropriate delay has passed:
    if (millis() - this->last_step_time >= step_delay) {
      // get the timeStamp of when you stepped:
      this->last_step_time = millis();
      // increment or decrement the step number,
      // depending on direction:
      if (this->direction == 1) {
        this->step_number++;
        if (this->step_number == this->number_of_steps) {
          this->step_number = 0;
        }
      } 
      else { 
        if (this->step_number == 0) {
          this->step_number = this->number_of_steps;
        }
        this->step_number--;
      }
      // decrement the steps left:
      steps_left--;

      // WAVE_DRIVE, FULL_STEP, HALF_STEP 
      if (step_type == HALF_STEP) {
        // step the motor to step number 0, 1, 2, 3, 4, 5, 6, 7, 8:
        stepMotor(this->step_number % 8, step_type);
      }
      else {
        // step the motor to step number 0, 1, 2, or 3:
        stepMotor(this->step_number % 4, step_type);
      }
    }
  }
}

/*
 * Moves the motor forward or backwards.
 */
void StepperAmperka::stepMotor(int thisStep, uint8_t step_type)
{
  if (step_type == FULL_STEP) {
    switch (thisStep) {
      case 3: /* 01 */
      digitalWrite(motor_pin_1, LOW);
      digitalWrite(motor_pin_4, HIGH);
      break;
      case 2: /* 11 */
      digitalWrite(motor_pin_1, HIGH);
      digitalWrite(motor_pin_4, HIGH);
      break;
      case 1: /* 10 */
      digitalWrite(motor_pin_1, HIGH);
      digitalWrite(motor_pin_4, LOW);
      break;
      case 0: /* 00 */
      digitalWrite(motor_pin_1, LOW);
      digitalWrite(motor_pin_4, LOW);
      break;
    } 
  } else if (step_type == WAVE_DRIVE) {
    switch (thisStep) {
      case 0:    // 1010
      digitalWrite(motor_pin_1, HIGH);
      digitalWrite(motor_pin_2, HIGH);
      digitalWrite(motor_pin_3, LOW);
      digitalWrite(motor_pin_4, HIGH);
      break;
      case 1:    // 0110
      digitalWrite(motor_pin_1, LOW);
      digitalWrite(motor_pin_2, LOW);
      digitalWrite(motor_pin_3, HIGH);
      digitalWrite(motor_pin_4, HIGH);
      break;
      case 2:    //0101
      digitalWrite(motor_pin_1, LOW);
      digitalWrite(motor_pin_2, HIGH);
      digitalWrite(motor_pin_3, LOW);
      digitalWrite(motor_pin_4, HIGH);
      break;
      case 3:    //1001
      digitalWrite(motor_pin_1, LOW);
      digitalWrite(motor_pin_2, LOW);
      digitalWrite(motor_pin_3, HIGH);
      digitalWrite(motor_pin_4, LOW);
      break;
    } 
  } else if (step_type == HALF_STEP) {
    switch (thisStep) {
      case 0:    // 1, 1, 0, 0
      digitalWrite(motor_pin_2, HIGH);
      digitalWrite(motor_pin_1, HIGH);
      digitalWrite(motor_pin_3, LOW);
      digitalWrite(motor_pin_4, LOW);
      break;
      case 1:    // 1, 1, 1, 1
      digitalWrite(motor_pin_2, HIGH);
      digitalWrite(motor_pin_1, HIGH);
      digitalWrite(motor_pin_3, HIGH);
      digitalWrite(motor_pin_4, HIGH);
      break;
      case 2:    //0, 0, 1, 1
      digitalWrite(motor_pin_2, LOW);
      digitalWrite(motor_pin_1, LOW);
      digitalWrite(motor_pin_3, HIGH);
      digitalWrite(motor_pin_4, HIGH);
      break;
      case 3:    //1, 0, 1, 1
      digitalWrite(motor_pin_2, HIGH);
      digitalWrite(motor_pin_1, LOW);
      digitalWrite(motor_pin_3, HIGH);
      digitalWrite(motor_pin_4, HIGH);
      break;
      
      case 4:    // 1, 0, 0, 0
      digitalWrite(motor_pin_2, HIGH);
      digitalWrite(motor_pin_1, LOW);
      digitalWrite(motor_pin_3, LOW);
      digitalWrite(motor_pin_4, LOW);
      break;
      case 5:    // 1, 0, 1, 0
      digitalWrite(motor_pin_2, HIGH);
      digitalWrite(motor_pin_1, LOW);
      digitalWrite(motor_pin_3, HIGH);
      digitalWrite(motor_pin_4, LOW);
      break;
      case 6:    //0, 0, 1, 0
      digitalWrite(motor_pin_2, LOW);
      digitalWrite(motor_pin_1, LOW);
      digitalWrite(motor_pin_3, HIGH);
      digitalWrite(motor_pin_4, LOW);
      break;
      case 7:    //1, 1, 1, 0
      digitalWrite(motor_pin_2, HIGH);
      digitalWrite(motor_pin_1, HIGH);
      digitalWrite(motor_pin_3, HIGH);
      digitalWrite(motor_pin_4, LOW);
      break;
    }
  }
}
