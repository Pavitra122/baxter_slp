//
// motor.h
// Header file for motor control portion of syringe pump project
//

#ifndef MOTOR_H
#define MOTOR_H

/* -- Includes -- */
#include <Arduino.h>
#include <stdint.h>


/* -- Enums -- */
enum MotorState
{
    STOPPED = 0,
    RUNNING = 1
};


/* Motor functions */
void     motorSetup   (void);                           // Perform setup needed for motor
uint32_t motorProcess (void);                           // Perform motor processing needed for every main loop cycle
                                                        //     returns       : delay (in usec) until next motor update needed
int      motorStart   (int16_t rate, uint8_t volume);   // Start the motor.  Parameters and return value:
                                                        //     rate   (input): rate to run motor at (in mL/hr), positive = forward, negative = reverse
                                                        //                     range: +/- 1 mL/hr to +/- 1,000 mL/hr
                                                        //     volume (input): amount of fluid to deliver (in mL)
                                                        //                     range: 1 mL to 30 mL
                                                        //     returns       : 0 if motor was started successfully, -1 if failure
void     motorStop    (void);                           // Stop the motor

#endif  // ifndef MOTOR_H
