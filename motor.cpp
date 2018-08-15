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
uint32_t    mMotorLoopDelay;          // Delay (in usec) until next motor update is needed


/* -- Functions -- */
//*******************************************************************************************
// Perform one-time motor setup at power up
//*******************************************************************************************
void motorSetup(void)
{
    // Setup motor hardware pins
    // Add code here

    // Initialize motor variables
    mMotorState = STOPPED;
    mMotorLoopDelay = DEFAULT_MOTOR_LOOP_DELAY;
}


//*******************************************************************************************
// Perform motor processing needed for every main loop cycle
// Returns: delay (in usec) until next motor update needed
//*******************************************************************************************
uint32_t motorProcess (void)
{
    // Check if motor is running
    if (mMotorState == RUNNING)
    {
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
}

#endif // MOTOR_SIMULATOR
