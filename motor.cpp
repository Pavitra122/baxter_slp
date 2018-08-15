//
// motor.cpp
// Code file for motor control portion of syringe pump project
//

#include "motor.h"
#include "ui.h"


//*******************************************************************************************
// Comment out the following line to use the real motor hardware!!!
//*******************************************************************************************
//#define MOTOR_SIMULATOR



//*******************************************************************************************
// Simulated motor software (for testing UI software)

#ifdef MOTOR_SIMULATOR

static bool infusing = false;
static uint32_t targetVolumeTenths = 0;
static uint32_t infusedVolumeTenths = 0;

void motorSetup(void) {}

uint32_t motorProcess(void)
{
  if (infusing)
  {
    infusedVolumeTenths++;
    infusing = (infusedVolumeTenths >= targetVolumeTenths) ? false : true;
    uiDeliveryStatus(infusing, infusedVolumeTenths);
  }

  return 1000000; // 1 second
}

int motorStart(int16_t /*rate*/, uint8_t volume)
{
  targetVolumeTenths = (uint32_t) volume * 10;
  infusedVolumeTenths = 0;
  infusing = true;
  return 0;
}

void motorStop(void)
{
  if (infusing)
  {
    infusing = false;
    uiDeliveryStatus(infusing, infusedVolumeTenths);
  }
}

// End of simulated motor software (for testing UI software)
//*******************************************************************************************




//*******************************************************************************************
// Real motor software (for running actual motor)

#else // else !MOTOR_SIMULATOR

/* -- Defines -- */
#define SYRINGE_VOLUME_ML          (30.0f)
#define SYRINGE_BARREL_LENGTH_MM   (80.0f)
#define THREADED_ROD_PITCH         (1.25f)
#define STEPS_PER_REVOLUTION       (200.0f)
#define MICROSTEPS_PER_STEP        (16.0f)


/* -- Constants -- */
const uint32_t DEFAULT_MOTOR_LOOP_DELAY = 10000UL;    // = 10000 usec (10 msec)


/* -- Pin definitions -- */
int motorDirPin   = 2;
int motorStepPin  = 3;


/* -- Variables -- */
MotorState  mMotorState;              // Motor state (stopped, running, etc.)
int16_t     mRate;                    // Infusion rate (in mL/hr)
uint16_t    mVolumeToBeInfused;       // Volume to be infused (in tenths of mL)
uint16_t    mVolumeInfused;           // Volume infused so far (in tenths of mL)
double    mMotorLoopDelay;          // Delay (in usec) until next motor update is needed


/* -- Functions -- */
//*******************************************************************************************
// Perform one-time motor setup at power up
//*******************************************************************************************
void motorSetup(void)
{
    // Setup motor hardware pins
    // Add code here
    pinMode(motorDirPin,OUTPUT);
    pinMode(motorStepPin,OUTPUT);
    //digitalWrite(motorDirPin,HIGH);
    // Initialize motor variables
    mMotorState = STOPPED;
    mMotorLoopDelay = DEFAULT_MOTOR_LOOP_DELAY;
//    while(1)
//    {
//
//      delayMicroseconds(240);
//      digitalWrite(motorStepPin,HIGH);
//      delayMicroseconds(240);
//      digitalWrite(motorStepPin,LOW);}

  
    /*while(1)
    {
        delayMicroseconds(500);
        digitalWrite(motorStepPin,HIGH);
        delayMicroseconds(500);
        digitalWrite(motorStepPin,LOW);
  
    }*/
}


//*******************************************************************************************
// Perform motor processing needed for every main loop cycle
// Returns: delay (in usec) until next motor update needed
//*******************************************************************************************
int highorLow = 1;
float volPerMicroStep = ( 1/ MICROSTEPS_PER_STEP)  * (1.0/ STEPS_PER_REVOLUTION) * (THREADED_ROD_PITCH) * (SYRINGE_VOLUME_ML / SYRINGE_BARREL_LENGTH_MM)/ 2.0;
float volumeInfused_temp = 0;
uint32_t motorProcess (void)
{
    // Check if motor is running
    if (mMotorState == RUNNING)
    {

            if (highorLow)
              digitalWrite(motorStepPin,HIGH);
            else
              digitalWrite(motorStepPin,LOW);
            highorLow = !highorLow;

            volumeInfused_temp += volPerMicroStep;

            if(volumeInfused_temp > mVolumeToBeInfused)
            {

                motorStop();
            }

      // Add code here
        // If running:
            // Pulse the motor (toggle the motor step pin)
            // Update number of microsteps infused (motor steps once per high/low transition on motor step pin)
            // Update volume infused
            // See if it is time to stop
            // Update the UI with the volume infused
    }

    // Return the delay until next motor update is needed
    return mMotorLoopDelay;
}


//*******************************************************************************************
// Start the motor
// Parameters:
//   rate   (input): rate to run motor at (in mL/hr), positive = forward, negative = reverse
//                     range: +/- 1 mL/hr to +/- 1,000 mL/hr
//   volume (input): amount of fluid to deliver (in mL)
//                     range: 1 mL to 30 mL
// Returns: 0 if motor was started successfully, -1 if failure
//*******************************************************************************************
int motorStart (int16_t rate, uint8_t volume)
{
   
    int retVal = 0;

    // Add code here
    if(mMotorState == STOPPED)
    {
        
        if(1 <= abs(rate) && abs(rate) <= 1000 && 1 <= volume && volume <= 30)
        {

            //mMotorLoopDelay = (1.0/abs((float)rate)) * (SYRINGE_VOLUME_ML / SYRINGE_BARREL_LENGTH_MM) * (THREADED_ROD_PITCH) * (1.0/ STEPS_PER_REVOLUTION) * ( 1/ MICROSTEPS_PER_STEP) * (3600000/2.0);
            mMotorLoopDelay = (1.0/abs((float)rate)) * 263.671 * 1000;
            if(rate > 0)
              digitalWrite(motorDirPin,HIGH);
            else
              digitalWrite(motorDirPin,LOW);
            mMotorState = RUNNING;

            mVolumeInfused = 0;
            volumeInfused_temp =0;
            mRate = rate;
            mVolumeToBeInfused = volume;
            return 0;

        }

        else return -1;

      
    }
    // If motor is stopped:
        // Perform range checking on the rate
        // Calculate loop delay that will give the correct infusion rate
        // Perform range checking on the volume to be infused
        // Reset the volume infused
        // Set the motor direction
        // Start the motor

    // Return value indicating if motor was started successfully
    return retVal;
}


//*******************************************************************************************
// Stop the motor
//*******************************************************************************************
void motorStop (void)
{
    // Add code here
    // Stop the motor
     mMotorState = STOPPED;
}

#endif // MOTOR_SIMULATOR
